# hahaOS  

hahaOS is a lightweight **RISC-V operating system** developed for educational and research purposes.  
Its goal is to explore **core OS design** (boot, scheduler, interrupts, memory management) while providing an experimental platform. 
Also, this project is mainly refer to [Operating System in 1000 Lines](https://operating-system-in-1000-lines.vercel.app/en/) and [linmo](https://github.com/sysprog21/linmo).

The development recorded at [作業系統開發紀錄](https://hackmd.io/@vicLin/HJa4KH5Uxl)


## Features

- **Scheduling**  
  - Cooperative scheduling (task voluntarily yields CPU)  
  - Preemptive scheduling (via timer interrupt)  
- **Memory management**  
  - BSS zeroing, heap initialization  
  - Simple dynamic allocator (free list with block coalescing)  
- **Context switching**  
  - Based on `jmp_buf` save/restore mechanism  
- **Basic drivers**  
  - UART 16550  
  - CLINT timer interrupt  


## Roadmap 
- [ ] O(1) complexity scheduler with different priority.
- [ ] linked list and queue data structure and related functions APIs.
- [ ] MMU and VM mechanism
- [ ] system calls
- [ ] SMP
- [ ] Synchronization primitives (spinlock, semaphore)
