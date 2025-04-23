import ida_segment
import ida_bytes
import ida_name
import idaapi

exports_segm = ida_segment.get_segm_by_name('exports')
assert exports_segm

exported_function_ea = ida_bytes.get_qword(exports_segm.start_ea + 0x08)
exported_function_count = ida_bytes.get_qword(exports_segm.start_ea + 0x10)

for i in range(exported_function_count):
    entry_ea = exported_function_ea + i * 0x10

    func_ea = ida_bytes.get_qword(entry_ea)
    func_name_ea = ida_bytes.get_qword(entry_ea + 0x08)

    func_name_managled = ida_bytes.get_strlit_contents(func_name_ea, -1, idaapi.STRTYPE_C).decode('utf-8')
    func_name_demangled = ida_name.demangle_name(func_name_managled, ida_name.MNG_LONG_FORM, ida_name.DQT_FULL)

    func_name_split = func_name_demangled.split('(')[0]
    #find last :: and get the function name
    simple_func_name = func_name_split.split('::')[-1]

    # add full type as comment
    ida_bytes.set_cmt(func_ea, func_name_demangled, 0)
    # set function name
    ida_name.set_name(func_ea, func_name_split)

    openbracket = "{"
    closebracket      = "}"
    print(f'{openbracket}&{simple_func_name}, "{func_name_managled}"{closebracket},')
