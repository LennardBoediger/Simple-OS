.balign 32
.global IVT
-IVT:
    b reset_tr
    b undef_tr
    #...

.global reset_tr