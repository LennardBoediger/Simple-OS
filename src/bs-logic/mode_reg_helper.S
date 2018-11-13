.global get_lr_sys
.global get_sp_sys
.global get_lr_svc
.global get_sp_svc
.global get_spsr_svc


get_lr_system:
    mrs r0, lr_usr
    bx lr

get_sp_system:
    mrs r0, sp_usr
    bx lr


get_lr_supervisor:
    mrs r0, lr_svc
    bx lr

get_sp_svc:
    mrs r0, sp_svc
    bx lr

get_spsr_svc:
    mrs r0, SPSR_svc
    bx lr

get_lr_abort:
    mrs r0, lr_abt
    bx lr

get_sp_svc:
    mrs r0, sp_svc
    bx lr

get_spsr_svc:
    mrs r0, SPSR_svc
    bx lr
get_lr_svc:
    mrs r0, lr_svc
    bx lr

get_sp_svc:
    mrs r0, sp_svc
    bx lr

get_spsr_svc:
    mrs r0, SPSR_svc
    bx lr
