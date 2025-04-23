# IDA fails at fixing up the chained fixups of the rosetta binary
# This script is a workaround to fix the chained fixups

import ida_segment
import ida_bytes
import idaapi
import ida_offset

'''
struct dyld_chained_starts_offsets
{
    uint32_t    pointer_format;     // DYLD_CHAINED_PTR_32_FIRMWARE
    uint32_t    starts_count;       // number of starts in array
    uint32_t    chain_starts[1];    // array chain start offsets
};

DYLD_CHAINED_PTR_64_OFFSET = 6


// DYLD_CHAINED_PTR_64/DYLD_CHAINED_PTR_64_OFFSET
struct dyld_chained_ptr_64_rebase
{
    uint64_t    target    : 36,    // 64GB max image size (DYLD_CHAINED_PTR_64 => vmAddr, DYLD_CHAINED_PTR_64_OFFSET => runtimeOffset)
                high8     :  8,    // top 8 bits set to this (DYLD_CHAINED_PTR_64 => after slide added, DYLD_CHAINED_PTR_64_OFFSET => before slide added)
                reserved  :  7,    // all zeros
                next      : 12,    // 4-byte stride
                bind      :  1;    // == 0
};

'''


chain_starts_segm = ida_segment.get_segm_by_name('__chain_starts')
assert chain_starts_segm

class dyld_chained_starts_offsets:
    def __init__(self, ea):
        self.pointer_format = ida_bytes.get_dword(ea)
        self.starts_count = ida_bytes.get_dword(ea + 4)
        self.chain_starts = []
        for i in range(self.starts_count):
            self.chain_starts.append(ida_bytes.get_dword(ea + 8 + i * 4))

    def __str__(self):
        hex_starts = [f"0x{x:08x}" for x in self.chain_starts]
        return f'pointer_format: 0x{self.pointer_format:x}, starts_count: {self.starts_count}, chain_starts: {hex_starts}'

class dyld_chained_ptr_64_rebase:
    def __init__(self, ea):
        self.target = ida_bytes.get_qword(ea) & 0xFFFFFFFFFF
        self.high8 = (ida_bytes.get_qword(ea) >> 36) & 0xFF
        self.reserved = (ida_bytes.get_qword(ea) >> 44) & 0x7F
        self.next = (ida_bytes.get_qword(ea) >> 52) & 0xFFF
        self.bind = (ida_bytes.get_qword(ea) >> 64) & 0x1

    def __str__(self):
        return f'target: 0x{self.target:09x}, high8: 0x{self.high8:02x}, reserved: 0x{self.reserved:02x}, next: 0x{self.next:03x}, bind: 0x{self.bind:01x}'

imagebase = idaapi.get_imagebase()
chained_starts = dyld_chained_starts_offsets(chain_starts_segm.start_ea)

def walk_chain(ea):
    print(f'walking chain at 0x{ea:08x}')
    is_next = True
    while is_next:
        chained_ptr = dyld_chained_ptr_64_rebase(ea)

        print(f' 0x{ea:08x}: {chained_ptr}')

        # patch the address
        ida_bytes.patch_qword(ea, chained_ptr.target + imagebase)
        # make the address an offset
        ida_bytes.create_data(ea, ida_bytes.FF_QWORD, 8, idaapi.BADADDR)
        ida_offset.op_plain_offset(ea, 0, 0)        

        ea += chained_ptr.next * 8
        is_next = chained_ptr.next != 0

for i in range(chained_starts.starts_count):
    walk_chain(chained_starts.chain_starts[i])