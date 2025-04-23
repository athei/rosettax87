extern "C" __attribute__((naked, used)) void _start() {
  asm volatile(
      ".globl start\n"
      "start:\n");
}