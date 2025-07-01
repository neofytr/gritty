## 🪛 Table of Contents

- [🪛 Table of Contents](#-table-of-contents)
- [1. 🧠 What Is Real Mode?](#1--what-is-real-mode)
- [2. 💾 What Is MS-DOS?](#2--what-is-ms-dos)
- [3. 🧱 The Intel i386 CPU and 32-bit Registers](#3--the-intel-i386-cpu-and-32-bit-registers)
  - [New 32-bit registers:](#new-32-bit-registers)
- [4. 🏷️ What Is `.code16gcc`?](#4-️-what-is-code16gcc)
- [5. 🛠 Why GCC Emits 32-bit Code in 16-bit Mode](#5--why-gcc-emits-32-bit-code-in-16-bit-mode)
- [6. 🧮 How 32-bit Instructions Work in Real Mode](#6--how-32-bit-instructions-work-in-real-mode)
- [7. 📞 Calling Conventions and GCC ABI](#7--calling-conventions-and-gcc-abi)
  - [🔹 ABI summary:](#-abi-summary)
  - [GCC Prologue:](#gcc-prologue)
  - [Epilogue:](#epilogue)
- [8. 🔁 Calling Assembly from C (and Vice Versa)](#8--calling-assembly-from-c-and-vice-versa)
  - [C → Assembly:](#c--assembly)
  - [Assembly → C:](#assembly--c)
- [9. 🏗 Using GCC to Build Real-Mode DOS Code](#9--using-gcc-to-build-real-mode-dos-code)
  - [✅ Compile C to Assembly:](#-compile-c-to-assembly)
  - [✅ Edit `file.s` to add:](#-edit-files-to-add)
  - [✅ Assemble:](#-assemble)
  - [✅ Link with LD:](#-link-with-ld)
  - [✅ Run:](#-run)
- [📄 `README.md`](#-readmemd)
  - [Step 2: Add `.code16gcc` to `main.s`](#step-2-add-code16gcc-to-mains)
  - [Step 3: Assemble](#step-3-assemble)
  - [Step 4: Link as Flat Binary](#step-4-link-as-flat-binary)
- [🏃 Running](#-running)
- [✅ Summary](#-summary)
- [💬 Notes](#-notes)

---

## 1. 🧠 What Is Real Mode?

Real Mode is the **startup mode** of all x86 CPUs.

| Property                | Value                              |
| ----------------------- | ---------------------------------- |
| Addressing              | 20-bit, via `segment:offset` pairs |
| Max memory accessible   | 1 MB (2²⁰ = 1,048,576 bytes)       |
| Memory protection       | ❌ None (flat, open access)         |
| Paging / Virtual Memory | ❌ Not supported                    |
| Instruction size        | Default is 16-bit                  |

All memory accesses are made using segments: `segment * 16 + offset`.

---

## 2. 💾 What Is MS-DOS?

MS-DOS is a **real-mode operating system**:

* Runs entirely in **real mode**
* Provides system services through `int 0x21`
* Programs and TSRs run as **real-mode binaries**
* Uses **16-bit registers and instructions**

✅ No protected mode, no paging, no multitasking

---

## 3. 🧱 The Intel i386 CPU and 32-bit Registers

Intel's **i386** (introduced in 1985) was the **first x86 CPU with full 32-bit support**.

### New 32-bit registers:

| 16-bit | 32-bit | Purpose             |
| ------ | ------ | ------------------- |
| `ax`   | `eax`  | Accumulator         |
| `bx`   | `ebx`  | Base register       |
| `sp`   | `esp`  | Stack pointer       |
| `bp`   | `ebp`  | Base/frame pointer  |
| `ip`   | `eip`  | Instruction pointer |

But even in **real mode**, the i386 **still starts in 16-bit mode**.

---

## 4. 🏷️ What Is `.code16gcc`?

`.code16gcc` is a **GAS (GNU Assembler)** directive that says:

> "Encode all instructions for real mode (16-bit), **but allow 32-bit instructions using prefixes**, because GCC assumes 32-bit ABI."

It enables:

* Writing code **compatible with real-mode DOS**
* Using **32-bit registers** (`eax`, `esp`, etc.)
* Matching GCC's expectations (32-bit stack and calling convention)

---

## 5. 🛠 Why GCC Emits 32-bit Code in 16-bit Mode

GCC assumes it’s always compiling for **32-bit targets**, even when using `-m16`.

So:

* It emits **`pushl %ebp`, `movl %esp, %ebp`, `ret`**, etc.
* Uses **32-bit stack offsets** (`[ebp + 8]`, `[ebp + 12]`)
* Outputs **32-bit instructions encoded for real mode**

Thus, GCC-generated `.code16gcc` code uses 32-bit stack frames **within 16-bit machine code** — made possible by **instruction prefixes**.

---

## 6. 🧮 How 32-bit Instructions Work in Real Mode

In real mode:

* **Default operand size** is 16-bit
* To use 32-bit operations, a **prefix `0x66`** is emitted before the instruction
* To use 32-bit addressing (like `[esp+4]`), use **prefix `0x67`**

✅ CPUs from **i386 and onward** can legally execute 32-bit instructions in real mode — it's part of the x86 ISA.

⚠️ **But BIOS and DOS do not expect them**, so be careful when mixing with interrupts or 16-bit code.

---

## 7. 📞 Calling Conventions and GCC ABI

GCC uses the **32-bit cdecl ABI**, even in `.code16gcc`.

### 🔹 ABI summary:

| Item           | Behavior                                  |
| -------------- | ----------------------------------------- |
| Args           | Passed on stack, right-to-left            |
| Stack frame    | `[ebp + 8]` = 1st arg, `[ebp + 12]` = 2nd |
| Return value   | In `eax`                                  |
| Caller cleanup | Yes                                       |

### GCC Prologue:

```asm
pushl %ebp
movl  %esp, %ebp
```

### Epilogue:

```asm
movl  %ebp, %esp
popl  %ebp
ret
```

---

## 8. 🔁 Calling Assembly from C (and Vice Versa)

### C → Assembly:

```c
extern void print_char(char ch);
print_char('A');
```

Assembly:

```asm
.global print_char
print_char:
	pushl %ebp
	movl  %esp, %ebp
	movb  8(%ebp), %dl
	movb  $0x02, %ah
	int   $0x21
	movl  %ebp, %esp
	popl  %ebp
	ret
```

### Assembly → C:

```asm
.extern my_c_func
call my_c_func
```

---

## 9. 🏗 Using GCC to Build Real-Mode DOS Code

### ✅ Compile C to Assembly:

```bash
gcc -m16 -ffreestanding -fno-pic -fno-stack-protector -O0 -S file.c -o file.s
```

### ✅ Edit `file.s` to add:

```asm
.code16gcc
```

### ✅ Assemble:

```bash
as --32 -o file.o file.s
```

### ✅ Link with LD:

`link.ld`:

```ld
OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS {
  . = 0x7C00;
  .text : { *(.text) }
  .data : { *(.data) }
}
```

```bash
ld -m elf_i386 -T link.ld -o prog.bin file.o
```

### ✅ Run:

```bash
qemu-system-i386 -fda prog.bin
```

---

## 📄 `README.md`

````md
# Real Mode DOS with GCC + `.code16gcc`

This project demonstrates how to use **GCC** to write 16-bit real-mode compatible code for **MS-DOS**, while still leveraging **32-bit features** like registers (`esp`, `ebp`) and the **cdecl ABI**.

---

## 🧠 Concepts

- **MS-DOS** runs in **real mode**, where default instruction size is 16-bit and only 1 MB memory is accessible.
- The **i386+ CPUs** support 32-bit registers and instructions, even in real mode, using special prefixes.
- `.code16gcc` allows GCC to emit **32-bit instructions** encoded for 16-bit mode.
- GCC expects the **32-bit cdecl calling convention**, even in real-mode output.

---

## 🛠 Building a Real-Mode Program

### Step 1: Compile C to Assembly
```bash
gcc -m16 -ffreestanding -fno-pic -fno-stack-protector -O0 -S main.c -o main.s
````

### Step 2: Add `.code16gcc` to `main.s`

At the very top:

```asm
.code16gcc
```

### Step 3: Assemble

```bash
as --32 -o main.o main.s
```

### Step 4: Link as Flat Binary

**link.ld:**

```ld
OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS {
  . = 0x7C00;
  .text : { *(.text) }
  .data : { *(.data) }
}
```

Then:

```bash
ld -m elf_i386 -T link.ld -o boot.bin main.o
```

---

## 🏃 Running

```bash
qemu-system-i386 -fda boot.bin
```

---

## ✅ Summary

| Feature                  | Supported                          |
| ------------------------ | ---------------------------------- |
| Real-mode compatible     | ✅ Yes                              |
| 32-bit registers (`eax`) | ✅ With `0x66` prefix               |
| GCC cdecl ABI            | ✅ Used by default in `.code16gcc`  |
| DOS interrupts (`int`)   | ✅ But use 16-bit regs (`ax`, `dl`) |
| Mixing C and ASM         | ✅ Easy with ABI match              |

---

## 💬 Notes

* Always ensure you **manually add `.code16gcc`** to your `.s` files.
* If you use BIOS/DOS interrupts, **use 16-bit registers only**.
* Don't use `pushl`/`popl` near interrupts unless you know what you're doing.

```
