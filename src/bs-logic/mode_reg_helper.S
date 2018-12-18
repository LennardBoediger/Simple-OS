.global get_lr_system
.global get_sp_system
.global get_lr_supervisor
.global get_sp_supervisor
.global get_spsr_supervisor
.global get_lr_abort
.global get_sp_abort
.global get_spsr_abort
.global get_lr_FIQ
.global get_sp_FIQ
.global get_spsr_FIQ
.global get_lr_IRQ
.global get_sp_IRQ
.global get_spsr_IRQ
.global get_lr_undefined
.global get_sp_undefined
.global get_spsr_undefined
.global get_cpsr

get_cpsr:
    mrs r0, cpsr
    bx lr

get_lr_system:
    mrs r0, lr_usr
    bx lr

get_sp_system:
    mrs r0, sp_usr
    bx lr


get_lr_supervisor:
    mrs r0, lr_svc
    bx lr

get_sp_supervisor:
    mrs r0, sp_svc
    bx lr

get_spsr_supervisor:
    mrs r0, SPSR_svc
    bx lr


get_lr_abort:
    mrs r0, lr_abt
    bx lr

get_sp_abort:
    mrs r0, sp_abt
    bx lr

get_spsr_abort:
    mrs r0, SPSR_abt
    bx lr


get_lr_FIQ:
    mrs r0, lr_fiq
    bx lr

get_sp_FIQ:
    mrs r0, sp_fiq
    bx lr

get_spsr_FIQ:
    mrs r0, SPSR_fiq
    bx lr


get_lr_IRQ:
    mrs r0, lr_irq
    bx lr

get_sp_IRQ:
    mrs r0, sp_irq
    bx lr

get_spsr_IRQ:
    mrs r0, SPSR_irq
    bx lr


get_lr_undefined:
    mrs r0, lr_und
    bx lr

get_sp_undefined:
    mrs r0, sp_und
    bx lr

get_spsr_undefined:
    mrs r0, SPSR_und
    bx lr
