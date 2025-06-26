	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.globl	_dsfmt_get_idstring             ## -- Begin function dsfmt_get_idstring
	.p2align	4, 0x90
_dsfmt_get_idstring:                    ## @dsfmt_get_idstring
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L_.str(%rip), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_dsfmt_get_min_array_size       ## -- Begin function dsfmt_get_min_array_size
	.p2align	4, 0x90
_dsfmt_get_min_array_size:              ## @dsfmt_get_min_array_size
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$382, %eax                      ## imm = 0x17E
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function dsfmt_gen_rand_all
LCPI2_0:
	.quad	4498102069230399                ## 0xffafffffffb3f
	.quad	4501400546508797                ## 0xffdfffc90fffd
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_gen_rand_all
	.p2align	4, 0x90
_dsfmt_gen_rand_all:                    ## @dsfmt_gen_rand_all
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	vmovdqa	3056(%rdi), %xmm0
	movq	$-2, %rax
	vmovdqa	LCPI2_0(%rip), %xmm1            ## xmm1 = [4498102069230399,4501400546508797]
	movq	%rdi, %rcx
	.p2align	4, 0x90
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%rcx), %xmm2
	vmovdqa	16(%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	1872(%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm0, %xmm7               ## xmm7 = xmm0[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	1888(%rcx), %xmm5, %xmm5
	vpxor	%xmm0, %xmm5, %xmm0
	vpxor	%xmm6, %xmm0, %xmm0
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm0, %xmm6
	vpand	%xmm1, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm1, %xmm0, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, (%rcx)
	vmovdqa	%xmm3, 16(%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$71, %rax
	jb	LBB2_1
## %bb.2:
	leaq	1200(%rdi), %rax
	movl	$72, %ecx
	.p2align	4, 0x90
LBB2_3:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rax), %xmm2
	vmovdqa	(%rax), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%rax), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm0, %xmm7               ## xmm7 = xmm0[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%rax), %xmm5, %xmm5
	vpxor	%xmm0, %xmm5, %xmm0
	vpxor	%xmm6, %xmm0, %xmm0
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm0, %xmm6
	vpand	%xmm1, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm1, %xmm0, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%rax)
	vmovdqa	%xmm3, (%rax)
	addq	$2, %rcx
	addq	$32, %rax
	cmpq	$188, %rcx
	jb	LBB2_3
## %bb.4:
	vmovdqa	3040(%rdi), %xmm1
	vpsllq	$19, %xmm1, %xmm2
	vpshufd	$27, %xmm0, %xmm0               ## xmm0 = xmm0[3,2,1,0]
	vpxor	1856(%rdi), %xmm0, %xmm0
	vpxor	%xmm2, %xmm0, %xmm0
	vpsrlq	$12, %xmm0, %xmm2
	vpand	LCPI2_0(%rip), %xmm0, %xmm3
	vpxor	%xmm1, %xmm2, %xmm1
	vpxor	%xmm3, %xmm1, %xmm1
	vmovdqa	%xmm1, 3040(%rdi)
	vmovdqa	%xmm0, 3056(%rdi)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function dsfmt_fill_array_close1_open2
LCPI3_0:
	.quad	4498102069230399                ## 0xffafffffffb3f
	.quad	4501400546508797                ## 0xffdfffc90fffd
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_fill_array_close1_open2
	.p2align	4, 0x90
_dsfmt_fill_array_close1_open2:         ## @dsfmt_fill_array_close1_open2
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	testb	$1, %dl
	jne	LBB3_36
## %bb.1:
	cmpq	$381, %rdx                      ## imm = 0x17D
	jle	LBB3_37
## %bb.2:
	vmovdqa	3056(%rdi), %xmm1
	movq	$-2, %rax
	xorl	%ecx, %ecx
	vmovdqa	LCPI3_0(%rip), %xmm0            ## xmm0 = [4498102069230399,4501400546508797]
	.p2align	4, 0x90
LBB3_3:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%rdi,%rcx), %xmm2
	vmovdqa	16(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	1872(%rdi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	1888(%rdi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, (%rsi,%rcx)
	vmovdqa	%xmm3, 16(%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$71, %rax
	jb	LBB3_3
## %bb.4:
	movl	$72, %eax
	movl	$1200, %ecx                     ## imm = 0x4B0
	.p2align	4, 0x90
LBB3_5:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rdi,%rcx), %xmm2
	vmovdqa	(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%rsi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%rsi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%rsi,%rcx)
	vmovdqa	%xmm3, (%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$188, %rax
	jb	LBB3_5
## %bb.6:
	movq	%rdx, %rax
	vmovdqa	3040(%rdi), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rsi), %xmm1, %xmm1
	vpxor	%xmm3, %xmm1, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI3_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, 3040(%rsi)
	shrq	%rax
	leaq	-191(%rax), %r8
	movl	$191, %ecx
	cmpq	$768, %rdx                      ## imm = 0x300
	jb	LBB3_9
## %bb.7:
	leaq	-193(%rax), %r9
	leaq	3072(%rsi), %r10
	.p2align	4, 0x90
LBB3_8:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-3072(%r10), %xmm2
	vmovdqa	-3056(%r10), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%r10), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%r10), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%r10)
	vmovdqa	%xmm3, (%r10)
	addq	$2, %rcx
	addq	$32, %r10
	cmpq	%r9, %rcx
	jle	LBB3_8
LBB3_9:
	cmpq	%r8, %rcx
	jge	LBB3_17
## %bb.10:
	leaq	-192(%rax), %r9
	leal	(%rax,%rcx), %r10d
	testb	$1, %r10b
	jne	LBB3_11
## %bb.12:
	movq	%rcx, %r10
	shlq	$4, %r10
	vmovdqa	-3056(%rsi,%r10), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%r10), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI3_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, (%rsi,%r10)
	leaq	1(%rcx), %r10
	cmpq	%rcx, %r9
	jne	LBB3_14
	jmp	LBB3_16
LBB3_11:
	movq	%rcx, %r10
	cmpq	%rcx, %r9
	je	LBB3_16
LBB3_14:
	movq	%rax, %rcx
	subq	%r10, %rcx
	shlq	$4, %r10
	leaq	(%r10,%rsi), %r9
	addq	$16, %r9
	addq	$-191, %rcx
	.p2align	4, 0x90
LBB3_15:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-3072(%r9), %xmm2
	vmovdqa	-3056(%r9), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1200(%r9), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm4, %xmm4
	vpxor	%xmm2, %xmm4, %xmm2
	vmovdqa	%xmm2, -16(%r9)
	vpsllq	$19, %xmm3, %xmm2
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%r9), %xmm2, %xmm2
	vpxor	%xmm1, %xmm2, %xmm1
	vpsrlq	$12, %xmm1, %xmm2
	vpand	%xmm0, %xmm1, %xmm4
	vpxor	%xmm4, %xmm2, %xmm2
	vpxor	%xmm3, %xmm2, %xmm2
	vmovdqa	%xmm2, (%r9)
	addq	$32, %r9
	addq	$-2, %rcx
	jne	LBB3_15
LBB3_16:
	movq	%r8, %rcx
LBB3_17:
	cmpq	$763, %rdx                      ## imm = 0x2FB
	ja	LBB3_18
## %bb.19:
	movq	%rax, %r10
	shlq	$4, %r10
	movl	$382, %r9d                      ## imm = 0x17E
	subq	%rax, %r9
	cmpq	$2, %r9
	movl	$1, %r8d
	cmovgeq	%r9, %r8
	movl	%r8d, %r9d
	andl	$3, %r9d
	cmpq	$756, %rdx                      ## imm = 0x2F4
	jbe	LBB3_34
## %bb.20:
	xorl	%edx, %edx
	jmp	LBB3_21
LBB3_18:
	xorl	%r8d, %r8d
	jmp	LBB3_24
LBB3_34:
	leaq	(%rsi,%r10), %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	movabsq	$9223372036854775804, %rbx      ## imm = 0x7FFFFFFFFFFFFFFC
	andq	%r8, %rbx
	movl	$48, %r14d
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB3_35:                                ## =>This Inner Loop Header: Depth=1
	vmovaps	-48(%r11,%r14), %xmm2
	vmovaps	%xmm2, -48(%rdi,%r14)
	vmovaps	-32(%r11,%r14), %xmm2
	vmovaps	%xmm2, -32(%rdi,%r14)
	vmovaps	-16(%r11,%r14), %xmm2
	vmovaps	%xmm2, -16(%rdi,%r14)
	vmovdqa	(%r11,%r14), %xmm2
	vmovdqa	%xmm2, (%rdi,%r14)
	addq	$4, %rdx
	addq	$64, %r14
	cmpq	%rdx, %rbx
	jne	LBB3_35
LBB3_21:
	testq	%r9, %r9
	je	LBB3_24
## %bb.22:
	shlq	$4, %rdx
	addq	%rdx, %r10
	addq	%rsi, %r10
	addq	$-3056, %r10                    ## imm = 0xF410
	addq	%rdi, %rdx
	shll	$4, %r9d
	xorl	%r11d, %r11d
	.p2align	4, 0x90
LBB3_23:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r10,%r11), %xmm2
	vmovdqa	%xmm2, (%rdx,%r11)
	addq	$16, %r11
	cmpq	%r11, %r9
	jne	LBB3_23
LBB3_24:
	leaq	-2(%rax), %rdx
	cmpq	%rdx, %rcx
	jg	LBB3_27
## %bb.25:
	movq	%rcx, %r9
	shlq	$4, %r9
	addq	%rsi, %r9
	movq	%r8, %r10
	shlq	$4, %r10
	addq	%rdi, %r10
	movl	$16, %r11d
	.p2align	4, 0x90
LBB3_26:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-3072(%r9,%r11), %xmm2
	vmovdqa	-3056(%r9,%r11), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%r9,%r11), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%r9,%r11), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%r9,%r11)
	vmovdqa	%xmm3, (%r9,%r11)
	vmovaps	-16(%r9,%r11), %xmm2
	vmovaps	%xmm2, -16(%r10,%r11)
	vmovdqa	(%r9,%r11), %xmm2
	vmovdqa	%xmm2, (%r10,%r11)
	addq	$2, %rcx
	addq	$2, %r8
	addq	$32, %r11
	cmpq	%rdx, %rcx
	jle	LBB3_26
LBB3_27:
	cmpq	%rcx, %rax
	jle	LBB3_33
## %bb.28:
	movl	%eax, %r9d
	subl	%ecx, %r9d
	leaq	1(%rcx), %rdx
	testb	$1, %r9b
	je	LBB3_30
## %bb.29:
	shlq	$4, %rcx
	vmovdqa	-3056(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%rcx), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI3_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, (%rsi,%rcx)
	movq	%r8, %r9
	shlq	$4, %r9
	vmovdqa	(%rsi,%rcx), %xmm2
	vmovdqa	%xmm2, (%rdi,%r9)
	incq	%r8
	movq	%rdx, %rcx
LBB3_30:
	cmpq	%rdx, %rax
	je	LBB3_33
## %bb.31:
	subq	%rcx, %rax
	shlq	$4, %rcx
	addq	%rcx, %rsi
	shlq	$4, %r8
	addq	%rdi, %r8
	movl	$16, %ecx
	.p2align	4, 0x90
LBB3_32:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-3072(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpxor	-1200(%rsi,%rcx), %xmm3, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	%xmm0, %xmm1, %xmm4
	vpxor	%xmm4, %xmm3, %xmm3
	vpxor	%xmm2, %xmm3, %xmm2
	vmovdqa	%xmm2, -16(%rsi,%rcx)
	vmovaps	-16(%rsi,%rcx), %xmm2
	vmovaps	%xmm2, -16(%r8,%rcx)
	vmovdqa	-3056(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%rcx), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	%xmm0, %xmm1, %xmm4
	vpxor	%xmm4, %xmm3, %xmm3
	vpxor	%xmm2, %xmm3, %xmm2
	vmovdqa	%xmm2, (%rsi,%rcx)
	vmovdqa	(%rsi,%rcx), %xmm2
	vmovdqa	%xmm2, (%r8,%rcx)
	addq	$32, %rcx
	addq	$-2, %rax
	jne	LBB3_32
LBB3_33:
	vmovdqa	%xmm1, 3056(%rdi)
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB3_36:
	callq	_dsfmt_fill_array_close1_open2.cold.1
LBB3_37:
	callq	_dsfmt_fill_array_close1_open2.cold.2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function dsfmt_fill_array_open_close
LCPI4_0:
	.quad	4498102069230399                ## 0xffafffffffb3f
	.quad	4501400546508797                ## 0xffdfffc90fffd
LCPI4_1:
	.quad	0x4000000000000000              ## double 2
	.quad	0x4000000000000000              ## double 2
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0
LCPI4_2:
	.quad	0x4000000000000000              ## double 2
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_fill_array_open_close
	.p2align	4, 0x90
_dsfmt_fill_array_open_close:           ## @dsfmt_fill_array_open_close
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	testb	$1, %dl
	jne	LBB4_39
## %bb.1:
	cmpq	$381, %rdx                      ## imm = 0x17D
	jle	LBB4_40
## %bb.2:
	vmovdqa	3056(%rdi), %xmm1
	movq	$-2, %rax
	xorl	%ecx, %ecx
	vmovdqa	LCPI4_0(%rip), %xmm0            ## xmm0 = [4498102069230399,4501400546508797]
	.p2align	4, 0x90
LBB4_3:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%rdi,%rcx), %xmm2
	vmovdqa	16(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	1872(%rdi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	1888(%rdi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, (%rsi,%rcx)
	vmovdqa	%xmm3, 16(%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$71, %rax
	jb	LBB4_3
## %bb.4:
	movl	$72, %eax
	movl	$1200, %ecx                     ## imm = 0x4B0
	.p2align	4, 0x90
LBB4_5:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rdi,%rcx), %xmm2
	vmovdqa	(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%rsi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%rsi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%rsi,%rcx)
	vmovdqa	%xmm3, (%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$188, %rax
	jb	LBB4_5
## %bb.6:
	movq	%rdx, %rax
	vmovdqa	3040(%rdi), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rsi), %xmm1, %xmm1
	vpxor	%xmm3, %xmm1, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI4_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, 3040(%rsi)
	shrq	%rax
	leaq	-191(%rax), %r8
	movl	$191, %ecx
	cmpq	$768, %rdx                      ## imm = 0x300
	jb	LBB4_9
## %bb.7:
	leaq	-193(%rax), %r9
	vmovddup	LCPI4_2(%rip), %xmm2            ## xmm2 = [2.0E+0,2.0E+0]
                                        ## xmm2 = mem[0,0]
	movq	%rsi, %r10
	.p2align	4, 0x90
LBB4_8:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r10), %xmm3
	vmovdqa	16(%r10), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1872(%r10), %xmm5, %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpshufd	$27, %xmm5, %xmm7               ## xmm7 = xmm5[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm8               ## xmm8 = xmm1[3,2,1,0]
	vpxor	%xmm5, %xmm8, %xmm5
	vpxor	1888(%r10), %xmm6, %xmm6
	vpxor	%xmm1, %xmm6, %xmm1
	vpxor	%xmm7, %xmm1, %xmm1
	vpsrlq	$12, %xmm5, %xmm6
	vpsrlq	$12, %xmm1, %xmm7
	vpand	%xmm0, %xmm5, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpxor	%xmm3, %xmm5, %xmm5
	vpxor	%xmm4, %xmm6, %xmm6
	vmovdqa	%xmm5, 3056(%r10)
	vmovdqa	%xmm6, 3072(%r10)
	vsubpd	%xmm3, %xmm2, %xmm3
	vmovapd	%xmm3, (%r10)
	vsubpd	%xmm4, %xmm2, %xmm3
	vmovapd	%xmm3, 16(%r10)
	addq	$2, %rcx
	addq	$32, %r10
	cmpq	%r9, %rcx
	jle	LBB4_8
LBB4_9:
	cmpq	%r8, %rcx
	jge	LBB4_17
## %bb.10:
	leaq	-192(%rax), %r9
	leal	(%rax,%rcx), %r10d
	testb	$1, %r10b
	jne	LBB4_11
## %bb.12:
	movq	%rcx, %r10
	shlq	$4, %r10
	vmovdqa	-3056(%rsi,%r10), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%r10), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI4_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm3
	vpxor	%xmm4, %xmm3, %xmm3
	vmovdqa	%xmm3, (%rsi,%r10)
	vmovddup	LCPI4_2(%rip), %xmm3            ## xmm3 = [2.0E+0,2.0E+0]
                                        ## xmm3 = mem[0,0]
	vsubpd	%xmm2, %xmm3, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%r10)
	leaq	1(%rcx), %r10
	cmpq	%rcx, %r9
	jne	LBB4_14
	jmp	LBB4_16
LBB4_11:
	movq	%rcx, %r10
	cmpq	%rcx, %r9
	je	LBB4_16
LBB4_14:
	movq	%rax, %rcx
	subq	%r10, %rcx
	shlq	$4, %r10
	leaq	(%r10,%rsi), %r9
	addq	$-3056, %r9                     ## imm = 0xF410
	addq	$-191, %rcx
	vmovddup	LCPI4_2(%rip), %xmm2            ## xmm2 = [2.0E+0,2.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB4_15:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r9), %xmm3
	vmovdqa	16(%r9), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1872(%r9), %xmm5, %xmm5
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	%xmm1, %xmm5, %xmm1
	vpsrlq	$12, %xmm1, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm5, %xmm5
	vpxor	%xmm3, %xmm5, %xmm5
	vmovdqa	%xmm5, 3056(%r9)
	vsubpd	%xmm3, %xmm2, %xmm3
	vmovapd	%xmm3, (%r9)
	vpsllq	$19, %xmm4, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1888(%r9), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm3, %xmm3
	vpxor	%xmm4, %xmm3, %xmm3
	vmovdqa	%xmm3, 3072(%r9)
	vsubpd	%xmm4, %xmm2, %xmm3
	vmovapd	%xmm3, 16(%r9)
	addq	$32, %r9
	addq	$-2, %rcx
	jne	LBB4_15
LBB4_16:
	movq	%r8, %rcx
LBB4_17:
	movq	%rax, %r8
	shlq	$4, %r8
	cmpq	$763, %rdx                      ## imm = 0x2FB
	ja	LBB4_18
## %bb.19:
	movl	$382, %r10d                     ## imm = 0x17E
	subq	%rax, %r10
	cmpq	$2, %r10
	movl	$1, %r9d
	cmovgeq	%r10, %r9
	movl	%r9d, %r10d
	andl	$3, %r10d
	cmpq	$756, %rdx                      ## imm = 0x2F4
	jbe	LBB4_36
## %bb.20:
	xorl	%edx, %edx
	jmp	LBB4_21
LBB4_18:
	xorl	%r9d, %r9d
	jmp	LBB4_24
LBB4_36:
	leaq	(%rsi,%r8), %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	movabsq	$9223372036854775804, %rbx      ## imm = 0x7FFFFFFFFFFFFFFC
	andq	%r9, %rbx
	movl	$48, %r14d
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB4_37:                                ## =>This Inner Loop Header: Depth=1
	vmovaps	-48(%r11,%r14), %xmm2
	vmovaps	%xmm2, -48(%rdi,%r14)
	vmovaps	-32(%r11,%r14), %xmm2
	vmovaps	%xmm2, -32(%rdi,%r14)
	vmovaps	-16(%r11,%r14), %xmm2
	vmovaps	%xmm2, -16(%rdi,%r14)
	vmovapd	(%r11,%r14), %xmm2
	vmovapd	%xmm2, (%rdi,%r14)
	addq	$4, %rdx
	addq	$64, %r14
	cmpq	%rdx, %rbx
	jne	LBB4_37
LBB4_21:
	testq	%r10, %r10
	je	LBB4_24
## %bb.22:
	shlq	$4, %rdx
	leaq	(%rdx,%r8), %r11
	addq	%rsi, %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	addq	%rdi, %rdx
	shll	$4, %r10d
	xorl	%ebx, %ebx
	.p2align	4, 0x90
LBB4_23:                                ## =>This Inner Loop Header: Depth=1
	vmovapd	(%r11,%rbx), %xmm2
	vmovapd	%xmm2, (%rdx,%rbx)
	addq	$16, %rbx
	cmpq	%rbx, %r10
	jne	LBB4_23
LBB4_24:
	leaq	-2(%rax), %rdx
	cmpq	%rdx, %rcx
	jg	LBB4_27
## %bb.25:
	movq	%rcx, %r10
	shlq	$4, %r10
	addq	%rsi, %r10
	addq	$-3056, %r10                    ## imm = 0xF410
	movq	%r9, %r11
	shlq	$4, %r11
	addq	%rdi, %r11
	movl	$16, %ebx
	vmovddup	LCPI4_2(%rip), %xmm2            ## xmm2 = [2.0E+0,2.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB4_26:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%r10,%rbx), %xmm3
	vmovdqa	(%r10,%rbx), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1856(%r10,%rbx), %xmm5, %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpshufd	$27, %xmm5, %xmm7               ## xmm7 = xmm5[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm8               ## xmm8 = xmm1[3,2,1,0]
	vpxor	%xmm5, %xmm8, %xmm5
	vpxor	1872(%r10,%rbx), %xmm6, %xmm6
	vpxor	%xmm1, %xmm6, %xmm1
	vpxor	%xmm7, %xmm1, %xmm1
	vpsrlq	$12, %xmm5, %xmm6
	vpsrlq	$12, %xmm1, %xmm7
	vpand	%xmm0, %xmm5, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpxor	%xmm3, %xmm5, %xmm3
	vpxor	%xmm4, %xmm6, %xmm4
	vmovdqa	%xmm3, 3040(%r10,%rbx)
	vmovdqa	%xmm4, 3056(%r10,%rbx)
	vmovaps	3040(%r10,%rbx), %xmm3
	vmovaps	%xmm3, -16(%r11,%rbx)
	vmovaps	3056(%r10,%rbx), %xmm3
	vmovaps	%xmm3, (%r11,%rbx)
	vsubpd	-16(%r10,%rbx), %xmm2, %xmm3
	vmovapd	%xmm3, -16(%r10,%rbx)
	vsubpd	(%r10,%rbx), %xmm2, %xmm3
	vmovapd	%xmm3, (%r10,%rbx)
	addq	$2, %rcx
	addq	$2, %r9
	addq	$32, %rbx
	cmpq	%rdx, %rcx
	jle	LBB4_26
LBB4_27:
	cmpq	%rcx, %rax
	jle	LBB4_33
## %bb.28:
	movl	%eax, %r10d
	subl	%ecx, %r10d
	leaq	1(%rcx), %rdx
	testb	$1, %r10b
	je	LBB4_30
## %bb.29:
	shlq	$4, %rcx
	vmovdqa	-3056(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%rcx), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI4_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, (%rsi,%rcx)
	movq	%r9, %r10
	shlq	$4, %r10
	vmovaps	(%rsi,%rcx), %xmm2
	vmovaps	%xmm2, (%rdi,%r10)
	vmovddup	LCPI4_2(%rip), %xmm2            ## xmm2 = [2.0E+0,2.0E+0]
                                        ## xmm2 = mem[0,0]
	vsubpd	-3056(%rsi,%rcx), %xmm2, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%rcx)
	incq	%r9
	movq	%rdx, %rcx
LBB4_30:
	cmpq	%rdx, %rax
	je	LBB4_33
## %bb.31:
	subq	%rcx, %rax
	shlq	$4, %rcx
	addq	%rsi, %rcx
	addq	$-3056, %rcx                    ## imm = 0xF410
	shlq	$4, %r9
	addq	%rdi, %r9
	movl	$16, %edx
	vmovddup	LCPI4_2(%rip), %xmm2            ## xmm2 = [2.0E+0,2.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB4_32:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rcx,%rdx), %xmm3
	vpsllq	$19, %xmm3, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rcx,%rdx), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm4, %xmm4
	vpxor	%xmm3, %xmm4, %xmm3
	vmovdqa	%xmm3, 3040(%rcx,%rdx)
	vmovaps	3040(%rcx,%rdx), %xmm3
	vmovaps	%xmm3, -16(%r9,%rdx)
	vsubpd	-16(%rcx,%rdx), %xmm2, %xmm3
	vmovapd	%xmm3, -16(%rcx,%rdx)
	vmovdqa	(%rcx,%rdx), %xmm3
	vpsllq	$19, %xmm3, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1872(%rcx,%rdx), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm4, %xmm4
	vpxor	%xmm3, %xmm4, %xmm3
	vmovdqa	%xmm3, 3056(%rcx,%rdx)
	vmovaps	3056(%rcx,%rdx), %xmm3
	vmovaps	%xmm3, (%r9,%rdx)
	vsubpd	(%rcx,%rdx), %xmm2, %xmm3
	vmovapd	%xmm3, (%rcx,%rdx)
	addq	$32, %rdx
	addq	$-2, %rax
	jne	LBB4_32
LBB4_33:
	addq	%r8, %rsi
	movq	$-2944, %rax                    ## imm = 0xF480
	vmovddup	LCPI4_2(%rip), %xmm0            ## xmm0 = [2.0E+0,2.0E+0]
                                        ## xmm0 = mem[0,0]
	.p2align	4, 0x90
LBB4_34:                                ## =>This Inner Loop Header: Depth=1
	vsubpd	-112(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -112(%rsi,%rax)
	vsubpd	-96(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -96(%rsi,%rax)
	vsubpd	-80(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -80(%rsi,%rax)
	vsubpd	-64(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -64(%rsi,%rax)
	vsubpd	-48(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -48(%rsi,%rax)
	vsubpd	-32(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -32(%rsi,%rax)
	vsubpd	-16(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, -16(%rsi,%rax)
	testq	%rax, %rax
	je	LBB4_38
## %bb.35:                              ##   in Loop: Header=BB4_34 Depth=1
	vsubpd	(%rsi,%rax), %xmm0, %xmm2
	vmovapd	%xmm2, (%rsi,%rax)
	subq	$-128, %rax
	jmp	LBB4_34
LBB4_38:
	vmovdqa	%xmm1, 3056(%rdi)
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB4_39:
	callq	_dsfmt_fill_array_open_close.cold.1
LBB4_40:
	callq	_dsfmt_fill_array_open_close.cold.2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function dsfmt_fill_array_close_open
LCPI5_0:
	.quad	4498102069230399                ## 0xffafffffffb3f
	.quad	4501400546508797                ## 0xffdfffc90fffd
LCPI5_1:
	.quad	0xbff0000000000000              ## double -1
	.quad	0xbff0000000000000              ## double -1
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0
LCPI5_2:
	.quad	0xbff0000000000000              ## double -1
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_fill_array_close_open
	.p2align	4, 0x90
_dsfmt_fill_array_close_open:           ## @dsfmt_fill_array_close_open
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	testb	$1, %dl
	jne	LBB5_39
## %bb.1:
	cmpq	$381, %rdx                      ## imm = 0x17D
	jle	LBB5_40
## %bb.2:
	vmovdqa	3056(%rdi), %xmm1
	movq	$-2, %rax
	xorl	%ecx, %ecx
	vmovdqa	LCPI5_0(%rip), %xmm0            ## xmm0 = [4498102069230399,4501400546508797]
	.p2align	4, 0x90
LBB5_3:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%rdi,%rcx), %xmm2
	vmovdqa	16(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	1872(%rdi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	1888(%rdi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, (%rsi,%rcx)
	vmovdqa	%xmm3, 16(%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$71, %rax
	jb	LBB5_3
## %bb.4:
	movl	$72, %eax
	movl	$1200, %ecx                     ## imm = 0x4B0
	.p2align	4, 0x90
LBB5_5:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rdi,%rcx), %xmm2
	vmovdqa	(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%rsi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%rsi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%rsi,%rcx)
	vmovdqa	%xmm3, (%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$188, %rax
	jb	LBB5_5
## %bb.6:
	movq	%rdx, %rax
	vmovdqa	3040(%rdi), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rsi), %xmm1, %xmm1
	vpxor	%xmm3, %xmm1, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI5_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, 3040(%rsi)
	shrq	%rax
	leaq	-191(%rax), %r8
	movl	$191, %ecx
	cmpq	$768, %rdx                      ## imm = 0x300
	jb	LBB5_9
## %bb.7:
	leaq	-193(%rax), %r9
	vmovddup	LCPI5_2(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	movq	%rsi, %r10
	.p2align	4, 0x90
LBB5_8:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r10), %xmm3
	vmovdqa	16(%r10), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1872(%r10), %xmm5, %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpshufd	$27, %xmm5, %xmm7               ## xmm7 = xmm5[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm8               ## xmm8 = xmm1[3,2,1,0]
	vpxor	%xmm5, %xmm8, %xmm5
	vpxor	1888(%r10), %xmm6, %xmm6
	vpxor	%xmm1, %xmm6, %xmm1
	vpxor	%xmm7, %xmm1, %xmm1
	vpsrlq	$12, %xmm5, %xmm6
	vpsrlq	$12, %xmm1, %xmm7
	vpand	%xmm0, %xmm5, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpxor	%xmm3, %xmm5, %xmm5
	vpxor	%xmm4, %xmm6, %xmm6
	vmovdqa	%xmm5, 3056(%r10)
	vmovdqa	%xmm6, 3072(%r10)
	vaddpd	%xmm2, %xmm3, %xmm3
	vmovapd	%xmm3, (%r10)
	vaddpd	%xmm2, %xmm4, %xmm3
	vmovapd	%xmm3, 16(%r10)
	addq	$2, %rcx
	addq	$32, %r10
	cmpq	%r9, %rcx
	jle	LBB5_8
LBB5_9:
	cmpq	%r8, %rcx
	jge	LBB5_17
## %bb.10:
	leaq	-192(%rax), %r9
	leal	(%rax,%rcx), %r10d
	testb	$1, %r10b
	jne	LBB5_11
## %bb.12:
	movq	%rcx, %r10
	shlq	$4, %r10
	vmovdqa	-3056(%rsi,%r10), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%r10), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI5_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm3
	vpxor	%xmm4, %xmm3, %xmm3
	vmovdqa	%xmm3, (%rsi,%r10)
	vaddpd	LCPI5_1(%rip), %xmm2, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%r10)
	leaq	1(%rcx), %r10
	cmpq	%rcx, %r9
	jne	LBB5_14
	jmp	LBB5_16
LBB5_11:
	movq	%rcx, %r10
	cmpq	%rcx, %r9
	je	LBB5_16
LBB5_14:
	movq	%rax, %rcx
	subq	%r10, %rcx
	shlq	$4, %r10
	leaq	(%r10,%rsi), %r9
	addq	$-3056, %r9                     ## imm = 0xF410
	addq	$-191, %rcx
	vmovddup	LCPI5_2(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB5_15:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r9), %xmm3
	vmovdqa	16(%r9), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1872(%r9), %xmm5, %xmm5
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	%xmm1, %xmm5, %xmm1
	vpsrlq	$12, %xmm1, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm5, %xmm5
	vpxor	%xmm3, %xmm5, %xmm5
	vmovdqa	%xmm5, 3056(%r9)
	vaddpd	%xmm2, %xmm3, %xmm3
	vmovapd	%xmm3, (%r9)
	vpsllq	$19, %xmm4, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1888(%r9), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm3, %xmm3
	vpxor	%xmm4, %xmm3, %xmm3
	vmovdqa	%xmm3, 3072(%r9)
	vaddpd	%xmm2, %xmm4, %xmm3
	vmovapd	%xmm3, 16(%r9)
	addq	$32, %r9
	addq	$-2, %rcx
	jne	LBB5_15
LBB5_16:
	movq	%r8, %rcx
LBB5_17:
	movq	%rax, %r8
	shlq	$4, %r8
	cmpq	$763, %rdx                      ## imm = 0x2FB
	ja	LBB5_18
## %bb.19:
	movl	$382, %r10d                     ## imm = 0x17E
	subq	%rax, %r10
	cmpq	$2, %r10
	movl	$1, %r9d
	cmovgeq	%r10, %r9
	movl	%r9d, %r10d
	andl	$3, %r10d
	cmpq	$756, %rdx                      ## imm = 0x2F4
	jbe	LBB5_36
## %bb.20:
	xorl	%edx, %edx
	jmp	LBB5_21
LBB5_18:
	xorl	%r9d, %r9d
	jmp	LBB5_24
LBB5_36:
	leaq	(%rsi,%r8), %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	movabsq	$9223372036854775804, %rbx      ## imm = 0x7FFFFFFFFFFFFFFC
	andq	%r9, %rbx
	movl	$48, %r14d
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB5_37:                                ## =>This Inner Loop Header: Depth=1
	vmovaps	-48(%r11,%r14), %xmm2
	vmovaps	%xmm2, -48(%rdi,%r14)
	vmovaps	-32(%r11,%r14), %xmm2
	vmovaps	%xmm2, -32(%rdi,%r14)
	vmovaps	-16(%r11,%r14), %xmm2
	vmovaps	%xmm2, -16(%rdi,%r14)
	vmovapd	(%r11,%r14), %xmm2
	vmovapd	%xmm2, (%rdi,%r14)
	addq	$4, %rdx
	addq	$64, %r14
	cmpq	%rdx, %rbx
	jne	LBB5_37
LBB5_21:
	testq	%r10, %r10
	je	LBB5_24
## %bb.22:
	shlq	$4, %rdx
	leaq	(%rdx,%r8), %r11
	addq	%rsi, %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	addq	%rdi, %rdx
	shll	$4, %r10d
	xorl	%ebx, %ebx
	.p2align	4, 0x90
LBB5_23:                                ## =>This Inner Loop Header: Depth=1
	vmovapd	(%r11,%rbx), %xmm2
	vmovapd	%xmm2, (%rdx,%rbx)
	addq	$16, %rbx
	cmpq	%rbx, %r10
	jne	LBB5_23
LBB5_24:
	leaq	-2(%rax), %rdx
	cmpq	%rdx, %rcx
	jg	LBB5_27
## %bb.25:
	movq	%rcx, %r10
	shlq	$4, %r10
	addq	%rsi, %r10
	addq	$-3056, %r10                    ## imm = 0xF410
	movq	%r9, %r11
	shlq	$4, %r11
	addq	%rdi, %r11
	movl	$16, %ebx
	vmovddup	LCPI5_2(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB5_26:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%r10,%rbx), %xmm3
	vmovdqa	(%r10,%rbx), %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpxor	1856(%r10,%rbx), %xmm5, %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpshufd	$27, %xmm5, %xmm7               ## xmm7 = xmm5[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm8               ## xmm8 = xmm1[3,2,1,0]
	vpxor	%xmm5, %xmm8, %xmm5
	vpxor	1872(%r10,%rbx), %xmm6, %xmm6
	vpxor	%xmm1, %xmm6, %xmm1
	vpxor	%xmm7, %xmm1, %xmm1
	vpsrlq	$12, %xmm5, %xmm6
	vpsrlq	$12, %xmm1, %xmm7
	vpand	%xmm0, %xmm5, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpxor	%xmm3, %xmm5, %xmm3
	vpxor	%xmm4, %xmm6, %xmm4
	vmovdqa	%xmm3, 3040(%r10,%rbx)
	vmovdqa	%xmm4, 3056(%r10,%rbx)
	vmovaps	3040(%r10,%rbx), %xmm3
	vmovaps	%xmm3, -16(%r11,%rbx)
	vmovaps	3056(%r10,%rbx), %xmm3
	vmovaps	%xmm3, (%r11,%rbx)
	vaddpd	-16(%r10,%rbx), %xmm2, %xmm3
	vmovapd	%xmm3, -16(%r10,%rbx)
	vaddpd	(%r10,%rbx), %xmm2, %xmm3
	vmovapd	%xmm3, (%r10,%rbx)
	addq	$2, %rcx
	addq	$2, %r9
	addq	$32, %rbx
	cmpq	%rdx, %rcx
	jle	LBB5_26
LBB5_27:
	cmpq	%rcx, %rax
	jle	LBB5_33
## %bb.28:
	movl	%eax, %r10d
	subl	%ecx, %r10d
	leaq	1(%rcx), %rdx
	testb	$1, %r10b
	je	LBB5_30
## %bb.29:
	shlq	$4, %rcx
	vmovdqa	-3056(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%rcx), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI5_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, (%rsi,%rcx)
	movq	%r9, %r10
	shlq	$4, %r10
	vmovaps	(%rsi,%rcx), %xmm2
	vmovaps	%xmm2, (%rdi,%r10)
	vmovapd	-3056(%rsi,%rcx), %xmm2
	vaddpd	LCPI5_1(%rip), %xmm2, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%rcx)
	incq	%r9
	movq	%rdx, %rcx
LBB5_30:
	cmpq	%rdx, %rax
	je	LBB5_33
## %bb.31:
	subq	%rcx, %rax
	shlq	$4, %rcx
	addq	%rsi, %rcx
	addq	$-3056, %rcx                    ## imm = 0xF410
	shlq	$4, %r9
	addq	%rdi, %r9
	movl	$16, %edx
	vmovddup	LCPI5_2(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB5_32:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rcx,%rdx), %xmm3
	vpsllq	$19, %xmm3, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rcx,%rdx), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm4, %xmm4
	vpxor	%xmm3, %xmm4, %xmm3
	vmovdqa	%xmm3, 3040(%rcx,%rdx)
	vmovaps	3040(%rcx,%rdx), %xmm3
	vmovaps	%xmm3, -16(%r9,%rdx)
	vaddpd	-16(%rcx,%rdx), %xmm2, %xmm3
	vmovapd	%xmm3, -16(%rcx,%rdx)
	vmovdqa	(%rcx,%rdx), %xmm3
	vpsllq	$19, %xmm3, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1872(%rcx,%rdx), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm4, %xmm4
	vpxor	%xmm3, %xmm4, %xmm3
	vmovdqa	%xmm3, 3056(%rcx,%rdx)
	vmovaps	3056(%rcx,%rdx), %xmm3
	vmovaps	%xmm3, (%r9,%rdx)
	vaddpd	(%rcx,%rdx), %xmm2, %xmm3
	vmovapd	%xmm3, (%rcx,%rdx)
	addq	$32, %rdx
	addq	$-2, %rax
	jne	LBB5_32
LBB5_33:
	addq	%r8, %rsi
	movq	$-2944, %rax                    ## imm = 0xF480
	vmovddup	LCPI5_2(%rip), %xmm0            ## xmm0 = [-1.0E+0,-1.0E+0]
                                        ## xmm0 = mem[0,0]
	vmovddup	LCPI5_2(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	.p2align	4, 0x90
LBB5_34:                                ## =>This Inner Loop Header: Depth=1
	vaddpd	-112(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -112(%rsi,%rax)
	vaddpd	-96(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -96(%rsi,%rax)
	vaddpd	-80(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -80(%rsi,%rax)
	vaddpd	-64(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -64(%rsi,%rax)
	vaddpd	-48(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -48(%rsi,%rax)
	vaddpd	-32(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -32(%rsi,%rax)
	vaddpd	-16(%rsi,%rax), %xmm0, %xmm3
	vmovapd	%xmm3, -16(%rsi,%rax)
	testq	%rax, %rax
	je	LBB5_38
## %bb.35:                              ##   in Loop: Header=BB5_34 Depth=1
	vaddpd	(%rsi,%rax), %xmm2, %xmm3
	vmovapd	%xmm3, (%rsi,%rax)
	subq	$-128, %rax
	jmp	LBB5_34
LBB5_38:
	vmovdqa	%xmm1, 3056(%rdi)
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB5_39:
	callq	_dsfmt_fill_array_close_open.cold.1
LBB5_40:
	callq	_dsfmt_fill_array_close_open.cold.2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ## -- Begin function dsfmt_fill_array_open_open
LCPI6_0:
	.quad	4498102069230399                ## 0xffafffffffb3f
	.quad	4501400546508797                ## 0xffdfffc90fffd
LCPI6_1:
	.quad	1                               ## 0x1
	.quad	1                               ## 0x1
LCPI6_2:
	.quad	0xbff0000000000000              ## double -1
	.quad	0xbff0000000000000              ## double -1
	.section	__TEXT,__const
LCPI6_3:
	.space	2,1
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0
LCPI6_4:
	.quad	0xbff0000000000000              ## double -1
LCPI6_5:
	.quad	1                               ## 0x1
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_fill_array_open_open
	.p2align	4, 0x90
_dsfmt_fill_array_open_open:            ## @dsfmt_fill_array_open_open
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	testb	$1, %dl
	jne	LBB6_39
## %bb.1:
	cmpq	$381, %rdx                      ## imm = 0x17D
	jle	LBB6_40
## %bb.2:
	vmovdqa	3056(%rdi), %xmm1
	movq	$-2, %rax
	xorl	%ecx, %ecx
	vmovdqa	LCPI6_0(%rip), %xmm0            ## xmm0 = [4498102069230399,4501400546508797]
	.p2align	4, 0x90
LBB6_3:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%rdi,%rcx), %xmm2
	vmovdqa	16(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	1872(%rdi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	1888(%rdi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, (%rsi,%rcx)
	vmovdqa	%xmm3, 16(%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$71, %rax
	jb	LBB6_3
## %bb.4:
	movl	$72, %eax
	movl	$1200, %ecx                     ## imm = 0x4B0
	.p2align	4, 0x90
LBB6_5:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rdi,%rcx), %xmm2
	vmovdqa	(%rdi,%rcx), %xmm3
	vpsllq	$19, %xmm2, %xmm4
	vpxor	-1200(%rsi,%rcx), %xmm4, %xmm4
	vpsllq	$19, %xmm3, %xmm5
	vpshufd	$27, %xmm4, %xmm6               ## xmm6 = xmm4[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm7               ## xmm7 = xmm1[3,2,1,0]
	vpxor	%xmm7, %xmm4, %xmm4
	vpxor	-1184(%rsi,%rcx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpxor	%xmm6, %xmm1, %xmm1
	vpsrlq	$12, %xmm4, %xmm5
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm4, %xmm4
	vpxor	%xmm4, %xmm5, %xmm4
	vpand	%xmm0, %xmm1, %xmm5
	vpxor	%xmm5, %xmm6, %xmm5
	vpxor	%xmm2, %xmm4, %xmm2
	vpxor	%xmm3, %xmm5, %xmm3
	vmovdqa	%xmm2, -16(%rsi,%rcx)
	vmovdqa	%xmm3, (%rsi,%rcx)
	addq	$2, %rax
	addq	$32, %rcx
	cmpq	$188, %rax
	jb	LBB6_5
## %bb.6:
	movq	%rdx, %rax
	vmovdqa	3040(%rdi), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rsi), %xmm1, %xmm1
	vpxor	%xmm3, %xmm1, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI6_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, 3040(%rsi)
	shrq	%rax
	leaq	-191(%rax), %r8
	movl	$191, %ecx
	cmpq	$768, %rdx                      ## imm = 0x300
	jb	LBB6_9
## %bb.7:
	leaq	-193(%rax), %r9
	vpmovsxbq	LCPI6_3(%rip), %xmm2    ## xmm2 = [1,1]
	vmovddup	LCPI6_4(%rip), %xmm3            ## xmm3 = [-1.0E+0,-1.0E+0]
                                        ## xmm3 = mem[0,0]
	movq	%rsi, %r10
	.p2align	4, 0x90
LBB6_8:                                 ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r10), %xmm4
	vmovdqa	16(%r10), %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpxor	1872(%r10), %xmm6, %xmm6
	vpsllq	$19, %xmm5, %xmm7
	vpshufd	$27, %xmm6, %xmm8               ## xmm8 = xmm6[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm9               ## xmm9 = xmm1[3,2,1,0]
	vpxor	%xmm6, %xmm9, %xmm6
	vpxor	1888(%r10), %xmm7, %xmm7
	vpxor	%xmm1, %xmm7, %xmm1
	vpxor	%xmm1, %xmm8, %xmm1
	vpsrlq	$12, %xmm6, %xmm7
	vpsrlq	$12, %xmm1, %xmm8
	vpand	%xmm0, %xmm6, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpand	%xmm0, %xmm1, %xmm7
	vpxor	%xmm7, %xmm8, %xmm7
	vpxor	%xmm4, %xmm6, %xmm6
	vpxor	%xmm5, %xmm7, %xmm7
	vmovdqa	%xmm6, 3056(%r10)
	vmovdqa	%xmm7, 3072(%r10)
	vpor	%xmm2, %xmm4, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, (%r10)
	vpor	%xmm2, %xmm5, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, 16(%r10)
	addq	$2, %rcx
	addq	$32, %r10
	cmpq	%r9, %rcx
	jle	LBB6_8
LBB6_9:
	cmpq	%r8, %rcx
	jge	LBB6_17
## %bb.10:
	leaq	-192(%rax), %r9
	leal	(%rax,%rcx), %r10d
	testb	$1, %r10b
	jne	LBB6_11
## %bb.12:
	movq	%rcx, %r10
	shlq	$4, %r10
	vmovdqa	-3056(%rsi,%r10), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%r10), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI6_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm3
	vpxor	%xmm4, %xmm3, %xmm3
	vmovdqa	%xmm3, (%rsi,%r10)
	vpor	LCPI6_1(%rip), %xmm2, %xmm2
	vaddpd	LCPI6_2(%rip), %xmm2, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%r10)
	leaq	1(%rcx), %r10
	cmpq	%rcx, %r9
	jne	LBB6_14
	jmp	LBB6_16
LBB6_11:
	movq	%rcx, %r10
	cmpq	%rcx, %r9
	je	LBB6_16
LBB6_14:
	movq	%rax, %rcx
	subq	%r10, %rcx
	shlq	$4, %r10
	leaq	(%r10,%rsi), %r9
	addq	$-3056, %r9                     ## imm = 0xF410
	addq	$-191, %rcx
	vpmovsxbq	LCPI6_3(%rip), %xmm2    ## xmm2 = [1,1]
	vmovddup	LCPI6_4(%rip), %xmm3            ## xmm3 = [-1.0E+0,-1.0E+0]
                                        ## xmm3 = mem[0,0]
	.p2align	4, 0x90
LBB6_15:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r9), %xmm4
	vmovdqa	16(%r9), %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1872(%r9), %xmm6, %xmm6
	vpxor	%xmm1, %xmm6, %xmm1
	vpsrlq	$12, %xmm1, %xmm6
	vpand	%xmm0, %xmm1, %xmm7
	vpxor	%xmm7, %xmm6, %xmm6
	vpxor	%xmm4, %xmm6, %xmm6
	vmovdqa	%xmm6, 3056(%r9)
	vpor	%xmm2, %xmm4, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, (%r9)
	vpsllq	$19, %xmm5, %xmm4
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1888(%r9), %xmm4, %xmm4
	vpxor	%xmm1, %xmm4, %xmm1
	vpsrlq	$12, %xmm1, %xmm4
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm4, %xmm4
	vpxor	%xmm5, %xmm4, %xmm4
	vmovdqa	%xmm4, 3072(%r9)
	vpor	%xmm2, %xmm5, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, 16(%r9)
	addq	$32, %r9
	addq	$-2, %rcx
	jne	LBB6_15
LBB6_16:
	movq	%r8, %rcx
LBB6_17:
	movq	%rax, %r8
	shlq	$4, %r8
	cmpq	$763, %rdx                      ## imm = 0x2FB
	ja	LBB6_18
## %bb.19:
	movl	$382, %r10d                     ## imm = 0x17E
	subq	%rax, %r10
	cmpq	$2, %r10
	movl	$1, %r9d
	cmovgeq	%r10, %r9
	movl	%r9d, %r10d
	andl	$3, %r10d
	cmpq	$756, %rdx                      ## imm = 0x2F4
	jbe	LBB6_36
## %bb.20:
	xorl	%edx, %edx
	jmp	LBB6_21
LBB6_18:
	xorl	%r9d, %r9d
	jmp	LBB6_24
LBB6_36:
	leaq	(%rsi,%r8), %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	movabsq	$9223372036854775804, %rbx      ## imm = 0x7FFFFFFFFFFFFFFC
	andq	%r9, %rbx
	movl	$48, %r14d
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB6_37:                                ## =>This Inner Loop Header: Depth=1
	vmovaps	-48(%r11,%r14), %xmm2
	vmovaps	%xmm2, -48(%rdi,%r14)
	vmovaps	-32(%r11,%r14), %xmm2
	vmovaps	%xmm2, -32(%rdi,%r14)
	vmovaps	-16(%r11,%r14), %xmm2
	vmovaps	%xmm2, -16(%rdi,%r14)
	vmovdqa	(%r11,%r14), %xmm2
	vmovdqa	%xmm2, (%rdi,%r14)
	addq	$4, %rdx
	addq	$64, %r14
	cmpq	%rdx, %rbx
	jne	LBB6_37
LBB6_21:
	testq	%r10, %r10
	je	LBB6_24
## %bb.22:
	shlq	$4, %rdx
	leaq	(%rdx,%r8), %r11
	addq	%rsi, %r11
	addq	$-3056, %r11                    ## imm = 0xF410
	addq	%rdi, %rdx
	shll	$4, %r10d
	xorl	%ebx, %ebx
	.p2align	4, 0x90
LBB6_23:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	(%r11,%rbx), %xmm2
	vmovdqa	%xmm2, (%rdx,%rbx)
	addq	$16, %rbx
	cmpq	%rbx, %r10
	jne	LBB6_23
LBB6_24:
	leaq	-2(%rax), %rdx
	cmpq	%rdx, %rcx
	jg	LBB6_27
## %bb.25:
	movq	%rcx, %r10
	shlq	$4, %r10
	addq	%rsi, %r10
	addq	$-3056, %r10                    ## imm = 0xF410
	movq	%r9, %r11
	shlq	$4, %r11
	addq	%rdi, %r11
	movl	$16, %ebx
	vmovddup	LCPI6_5(%rip), %xmm2            ## xmm2 = [1,1]
                                        ## xmm2 = mem[0,0]
	vmovddup	LCPI6_4(%rip), %xmm3            ## xmm3 = [-1.0E+0,-1.0E+0]
                                        ## xmm3 = mem[0,0]
	.p2align	4, 0x90
LBB6_26:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%r10,%rbx), %xmm4
	vmovdqa	(%r10,%rbx), %xmm5
	vpsllq	$19, %xmm4, %xmm6
	vpxor	1856(%r10,%rbx), %xmm6, %xmm6
	vpsllq	$19, %xmm5, %xmm7
	vpshufd	$27, %xmm6, %xmm8               ## xmm8 = xmm6[3,2,1,0]
	vpshufd	$27, %xmm1, %xmm9               ## xmm9 = xmm1[3,2,1,0]
	vpxor	%xmm6, %xmm9, %xmm6
	vpxor	1872(%r10,%rbx), %xmm7, %xmm7
	vpxor	%xmm1, %xmm7, %xmm1
	vpxor	%xmm1, %xmm8, %xmm1
	vpsrlq	$12, %xmm6, %xmm7
	vpsrlq	$12, %xmm1, %xmm8
	vpand	%xmm0, %xmm6, %xmm6
	vpxor	%xmm6, %xmm7, %xmm6
	vpand	%xmm0, %xmm1, %xmm7
	vpxor	%xmm7, %xmm8, %xmm7
	vpxor	%xmm4, %xmm6, %xmm4
	vpxor	%xmm5, %xmm7, %xmm5
	vmovdqa	%xmm4, 3040(%r10,%rbx)
	vmovdqa	%xmm5, 3056(%r10,%rbx)
	vmovaps	3040(%r10,%rbx), %xmm4
	vmovaps	%xmm4, -16(%r11,%rbx)
	vmovaps	3056(%r10,%rbx), %xmm4
	vmovaps	%xmm4, (%r11,%rbx)
	vorpd	-16(%r10,%rbx), %xmm2, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, -16(%r10,%rbx)
	vorpd	(%r10,%rbx), %xmm2, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, (%r10,%rbx)
	addq	$2, %rcx
	addq	$2, %r9
	addq	$32, %rbx
	cmpq	%rdx, %rcx
	jle	LBB6_26
LBB6_27:
	cmpq	%rcx, %rax
	jle	LBB6_33
## %bb.28:
	movl	%eax, %r10d
	subl	%ecx, %r10d
	leaq	1(%rcx), %rdx
	testb	$1, %r10b
	je	LBB6_30
## %bb.29:
	shlq	$4, %rcx
	vmovdqa	-3056(%rsi,%rcx), %xmm2
	vpsllq	$19, %xmm2, %xmm3
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	-1184(%rsi,%rcx), %xmm3, %xmm3
	vpxor	%xmm1, %xmm3, %xmm1
	vpsrlq	$12, %xmm1, %xmm3
	vpand	LCPI6_0(%rip), %xmm1, %xmm4
	vpxor	%xmm2, %xmm3, %xmm2
	vpxor	%xmm4, %xmm2, %xmm2
	vmovdqa	%xmm2, (%rsi,%rcx)
	movq	%r9, %r10
	shlq	$4, %r10
	vmovaps	(%rsi,%rcx), %xmm2
	vmovaps	%xmm2, (%rdi,%r10)
	vmovapd	-3056(%rsi,%rcx), %xmm2
	vorpd	LCPI6_1(%rip), %xmm2, %xmm2
	vaddpd	LCPI6_2(%rip), %xmm2, %xmm2
	vmovapd	%xmm2, -3056(%rsi,%rcx)
	incq	%r9
	movq	%rdx, %rcx
LBB6_30:
	cmpq	%rdx, %rax
	je	LBB6_33
## %bb.31:
	subq	%rcx, %rax
	shlq	$4, %rcx
	addq	%rsi, %rcx
	addq	$-3056, %rcx                    ## imm = 0xF410
	shlq	$4, %r9
	addq	%rdi, %r9
	movl	$16, %edx
	vmovddup	LCPI6_5(%rip), %xmm2            ## xmm2 = [1,1]
                                        ## xmm2 = mem[0,0]
	vmovddup	LCPI6_4(%rip), %xmm3            ## xmm3 = [-1.0E+0,-1.0E+0]
                                        ## xmm3 = mem[0,0]
	.p2align	4, 0x90
LBB6_32:                                ## =>This Inner Loop Header: Depth=1
	vmovdqa	-16(%rcx,%rdx), %xmm4
	vpsllq	$19, %xmm4, %xmm5
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	1856(%rcx,%rdx), %xmm5, %xmm5
	vpxor	%xmm1, %xmm5, %xmm1
	vpsrlq	$12, %xmm1, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm5, %xmm5
	vpxor	%xmm4, %xmm5, %xmm4
	vmovdqa	%xmm4, 3040(%rcx,%rdx)
	vmovaps	3040(%rcx,%rdx), %xmm4
	vmovaps	%xmm4, -16(%r9,%rdx)
	vorpd	-16(%rcx,%rdx), %xmm2, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, -16(%rcx,%rdx)
	vmovdqa	(%rcx,%rdx), %xmm4
	vpsllq	$19, %xmm4, %xmm5
	vpxor	1872(%rcx,%rdx), %xmm5, %xmm5
	vpshufd	$27, %xmm1, %xmm1               ## xmm1 = xmm1[3,2,1,0]
	vpxor	%xmm1, %xmm5, %xmm1
	vpsrlq	$12, %xmm1, %xmm5
	vpand	%xmm0, %xmm1, %xmm6
	vpxor	%xmm6, %xmm5, %xmm5
	vpxor	%xmm4, %xmm5, %xmm4
	vmovdqa	%xmm4, 3056(%rcx,%rdx)
	vmovaps	3056(%rcx,%rdx), %xmm4
	vmovaps	%xmm4, (%r9,%rdx)
	vorpd	(%rcx,%rdx), %xmm2, %xmm4
	vaddpd	%xmm3, %xmm4, %xmm4
	vmovapd	%xmm4, (%rcx,%rdx)
	addq	$32, %rdx
	addq	$-2, %rax
	jne	LBB6_32
LBB6_33:
	addq	%r8, %rsi
	movq	$-3008, %rax                    ## imm = 0xF440
	vmovddup	LCPI6_5(%rip), %xmm0            ## xmm0 = [1,1]
                                        ## xmm0 = mem[0,0]
	vmovddup	LCPI6_4(%rip), %xmm2            ## xmm2 = [-1.0E+0,-1.0E+0]
                                        ## xmm2 = mem[0,0]
	vmovddup	LCPI6_5(%rip), %xmm3            ## xmm3 = [1,1]
                                        ## xmm3 = mem[0,0]
	vmovddup	LCPI6_4(%rip), %xmm4            ## xmm4 = [-1.0E+0,-1.0E+0]
                                        ## xmm4 = mem[0,0]
	.p2align	4, 0x90
LBB6_34:                                ## =>This Inner Loop Header: Depth=1
	vorpd	-48(%rsi,%rax), %xmm0, %xmm5
	vaddpd	%xmm2, %xmm5, %xmm5
	vmovapd	%xmm5, -48(%rsi,%rax)
	vorpd	-32(%rsi,%rax), %xmm0, %xmm5
	vaddpd	%xmm2, %xmm5, %xmm5
	vmovapd	%xmm5, -32(%rsi,%rax)
	vorpd	-16(%rsi,%rax), %xmm0, %xmm5
	vaddpd	%xmm2, %xmm5, %xmm5
	vmovapd	%xmm5, -16(%rsi,%rax)
	testq	%rax, %rax
	je	LBB6_38
## %bb.35:                              ##   in Loop: Header=BB6_34 Depth=1
	vorpd	(%rsi,%rax), %xmm3, %xmm5
	vaddpd	%xmm4, %xmm5, %xmm5
	vmovapd	%xmm5, (%rsi,%rax)
	addq	$64, %rax
	jmp	LBB6_34
LBB6_38:
	vmovdqa	%xmm1, 3056(%rdi)
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB6_39:
	callq	_dsfmt_fill_array_open_open.cold.1
LBB6_40:
	callq	_dsfmt_fill_array_open_open.cold.2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0                          ## -- Begin function dsfmt_chk_init_gen_rand
LCPI7_0:
	.quad	4503599627370495                ## 0xfffffffffffff
LCPI7_1:
	.quad	4607182418800017408             ## 0x3ff0000000000000
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_chk_init_gen_rand
	.p2align	4, 0x90
_dsfmt_chk_init_gen_rand:               ## @dsfmt_chk_init_gen_rand
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
                                        ## kill: def $esi killed $esi def $rsi
	cmpl	$19937, %edx                    ## imm = 0x4DE1
	jne	LBB7_9
## %bb.1:
	movl	%esi, (%rdi)
	movl	$1, %eax
	xorl	%ecx, %ecx
	.p2align	4, 0x90
LBB7_2:                                 ## =>This Inner Loop Header: Depth=1
	movl	%esi, %edx
	shrl	$30, %edx
	xorl	%esi, %edx
	imull	$1812433253, %edx, %edx         ## imm = 0x6C078965
	addl	%ecx, %edx
	incl	%edx
	movl	%edx, 4(%rdi,%rcx,4)
	movl	%edx, %esi
	shrl	$30, %esi
	xorl	%edx, %esi
	imull	$1812433253, %esi, %edx         ## imm = 0x6C078965
	addl	%ecx, %edx
	addl	$2, %edx
	movl	%edx, 8(%rdi,%rcx,4)
	movl	%edx, %esi
	shrl	$30, %esi
	xorl	%edx, %esi
	imull	$1812433253, %esi, %edx         ## imm = 0x6C078965
	addq	%rcx, %rdx
	addq	$3, %rdx
	movl	%edx, 12(%rdi,%rcx,4)
	cmpq	$764, %rcx                      ## imm = 0x2FC
	je	LBB7_3
## %bb.10:                              ##   in Loop: Header=BB7_2 Depth=1
	leaq	3(%rax), %rsi
	movl	%edx, %r8d
	shrl	$30, %r8d
	xorl	%edx, %r8d
	imull	$1812433253, %r8d, %edx         ## imm = 0x6C078965
	addl	%edx, %esi
	addl	%ecx, %edx
	addl	$4, %edx
	movl	%edx, 16(%rdi,%rcx,4)
	addq	$4, %rax
	addq	$4, %rcx
	jmp	LBB7_2
LBB7_3:
	movl	$28, %eax
	vbroadcastsd	LCPI7_0(%rip), %ymm0    ## ymm0 = [4503599627370495,4503599627370495,4503599627370495,4503599627370495]
	vbroadcastsd	LCPI7_1(%rip), %ymm1    ## ymm1 = [4607182418800017408,4607182418800017408,4607182418800017408,4607182418800017408]
	.p2align	4, 0x90
LBB7_4:                                 ## =>This Inner Loop Header: Depth=1
	vandps	-224(%rdi,%rax,8), %ymm0, %ymm2
	vandps	-192(%rdi,%rax,8), %ymm0, %ymm3
	vandps	-160(%rdi,%rax,8), %ymm0, %ymm4
	vandps	-128(%rdi,%rax,8), %ymm0, %ymm5
	vorps	%ymm1, %ymm2, %ymm2
	vorps	%ymm1, %ymm3, %ymm3
	vorps	%ymm1, %ymm4, %ymm4
	vmovups	%ymm2, -224(%rdi,%rax,8)
	vmovups	%ymm3, -192(%rdi,%rax,8)
	vmovups	%ymm4, -160(%rdi,%rax,8)
	vorps	%ymm1, %ymm5, %ymm2
	vmovups	%ymm2, -128(%rdi,%rax,8)
	cmpq	$380, %rax                      ## imm = 0x17C
	je	LBB7_6
## %bb.5:                               ##   in Loop: Header=BB7_4 Depth=1
	vandps	-96(%rdi,%rax,8), %ymm0, %ymm2
	vandps	-64(%rdi,%rax,8), %ymm0, %ymm3
	vandps	-32(%rdi,%rax,8), %ymm0, %ymm4
	vandps	(%rdi,%rax,8), %ymm0, %ymm5
	vorps	%ymm1, %ymm2, %ymm2
	vorps	%ymm1, %ymm3, %ymm3
	vorps	%ymm1, %ymm4, %ymm4
	vmovups	%ymm2, -96(%rdi,%rax,8)
	vmovups	%ymm3, -64(%rdi,%rax,8)
	vmovups	%ymm4, -32(%rdi,%rax,8)
	vorps	%ymm1, %ymm5, %ymm2
	vmovups	%ymm2, (%rdi,%rax,8)
	addq	$32, %rax
	jmp	LBB7_4
LBB7_6:
	movb	$52, %al
	bzhiq	%rax, 2944(%rdi), %rdx
	movabsq	$4607182418800017408, %rcx      ## imm = 0x3FF0000000000000
	orq	%rcx, %rdx
	movq	%rdx, 2944(%rdi)
	bzhiq	%rax, 2952(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2952(%rdi)
	bzhiq	%rax, 2960(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2960(%rdi)
	bzhiq	%rax, 2968(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2968(%rdi)
	bzhiq	%rax, 2976(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2976(%rdi)
	bzhiq	%rax, 2984(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2984(%rdi)
	bzhiq	%rax, 2992(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2992(%rdi)
	bzhiq	%rax, 3000(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3000(%rdi)
	bzhiq	%rax, 3008(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3008(%rdi)
	bzhiq	%rax, 3016(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3016(%rdi)
	bzhiq	%rax, 3024(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3024(%rdi)
	bzhiq	%rax, 3032(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3032(%rdi)
	bzhiq	%rax, 3040(%rdi), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3040(%rdi)
	bzhiq	%rax, 3048(%rdi), %rax
	orq	%rcx, %rax
	movq	%rax, 3048(%rdi)
	movq	3064(%rdi), %rax
	movabsq	$4432916062321256576, %rcx      ## imm = 0x3D84E1AC0DC82880
	andq	3056(%rdi), %rcx
	movl	%eax, %edx
	andl	$1, %edx
	orq	%rcx, %rdx
	movabsq	$1152993127498776576, %rcx      ## imm = 0x1000412401080000
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$32, %rdx
	xorq	%rcx, %rdx
	movq	%rdx, %rcx
	shrq	$16, %rcx
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$8, %rdx
	xorq	%rcx, %rdx
	movq	%rdx, %rcx
	shrq	$4, %rcx
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$2, %rdx
	xorl	%ecx, %edx
	movl	%edx, %ecx
	shrl	%ecx
	xorl	%edx, %ecx
	testb	$1, %cl
	jne	LBB7_8
## %bb.7:
	xorq	$1, %rax
	movq	%rax, 3064(%rdi)
LBB7_8:
	movl	$382, 3072(%rdi)                ## imm = 0x17E
	popq	%rbp
	vzeroupper
	retq
LBB7_9:
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	leaq	L_.str.4(%rip), %rdi
	movl	$38, %esi
	movl	$1, %edx
	callq	_fwrite
	movl	$1, %edi
	callq	_exit
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0                          ## -- Begin function dsfmt_chk_init_by_array
LCPI8_0:
	.quad	4503599627370495                ## 0xfffffffffffff
LCPI8_1:
	.quad	4607182418800017408             ## 0x3ff0000000000000
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_dsfmt_chk_init_by_array
	.p2align	4, 0x90
_dsfmt_chk_init_by_array:               ## @dsfmt_chk_init_by_array
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	cmpl	$19937, %ecx                    ## imm = 0x4DE1
	jne	LBB8_16
## %bb.1:
	movl	%edx, %r14d
	movq	%rsi, %r15
	movq	%rdi, %rbx
	movl	$3072, %edx                     ## imm = 0xC00
	movl	$139, %esi
	callq	_memset
	cmpl	$768, %r14d                     ## imm = 0x300
	movl	$767, %eax                      ## imm = 0x2FF
	cmovgel	%r14d, %eax
	movl	1512(%rbx), %ecx
	movl	3068(%rbx), %edx
	xorl	%ecx, %edx
	xorl	$-1953789045, %edx              ## imm = 0x8B8B8B8B
	movl	%edx, %esi
	shrl	$27, %esi
	xorl	%edx, %esi
	imull	$1664525, %esi, %edx            ## imm = 0x19660D
	addl	%edx, %ecx
	movl	%ecx, 1512(%rbx)
	addl	%r14d, %edx
	addl	%edx, 1556(%rbx)
	movl	%edx, (%rbx)
	testl	%r14d, %r14d
	jle	LBB8_2
## %bb.11:
	movl	%r14d, %ecx
	movl	$1, %esi
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB8_12:                                ## =>This Inner Loop Header: Depth=1
	movslq	%esi, %rsi
	movl	(%rbx,%rsi,4), %edi
	leal	378(%rsi), %r8d
	movslq	%r8d, %r8
	imulq	$715827883, %r8, %r8            ## imm = 0x2AAAAAAB
	movq	%r8, %r9
	shrq	$63, %r9
	shrq	$39, %r8
	addl	%r9d, %r8d
	shll	$8, %r8d
	leal	(%r8,%r8,2), %r8d
	negl	%r8d
	addl	%esi, %r8d
	addl	$378, %r8d                      ## imm = 0x17A
	movslq	%r8d, %r8
	movl	(%rbx,%r8,4), %r9d
	leal	767(%rsi), %r10d
	movslq	%r10d, %r10
	imulq	$715827883, %r10, %r10          ## imm = 0x2AAAAAAB
	movq	%r10, %r11
	shrq	$63, %r11
	shrq	$39, %r10
	addl	%r11d, %r10d
	shll	$8, %r10d
	leal	(%r10,%r10,2), %r10d
	negl	%r10d
	addl	%esi, %r10d
	addl	$767, %r10d                     ## imm = 0x2FF
	movslq	%r10d, %r10
	xorl	(%rbx,%r10,4), %edi
	xorl	%r9d, %edi
	movl	%edi, %r10d
	shrl	$27, %r10d
	xorl	%edi, %r10d
	imull	$1664525, %r10d, %edi           ## imm = 0x19660D
	addl	%edi, %r9d
	movl	%r9d, (%rbx,%r8,4)
	movl	(%r15,%rdx,4), %r8d
	addl	%esi, %r8d
	addl	%edi, %r8d
	leal	389(%rsi), %edi
	movslq	%edi, %rdi
	imulq	$715827883, %rdi, %rdi          ## imm = 0x2AAAAAAB
	movq	%rdi, %r9
	shrq	$63, %r9
	shrq	$39, %rdi
	addl	%r9d, %edi
	shll	$8, %edi
	leal	(%rdi,%rdi,2), %edi
	negl	%edi
	addl	%esi, %edi
	addl	$389, %edi                      ## imm = 0x185
	movslq	%edi, %rdi
	addl	%r8d, (%rbx,%rdi,4)
	movl	%r8d, (%rbx,%rsi,4)
	leal	1(%rsi), %edi
	movslq	%edi, %rdi
	imulq	$715827883, %rdi, %rdi          ## imm = 0x2AAAAAAB
	movq	%rdi, %r8
	shrq	$63, %r8
	shrq	$39, %rdi
	addl	%r8d, %edi
	shll	$8, %edi
	leal	(%rdi,%rdi,2), %edi
	negl	%edi
	addl	%edi, %esi
	incl	%esi
	incq	%rdx
	cmpq	%rdx, %rcx
	jne	LBB8_12
## %bb.3:
	cmpl	$766, %r14d                     ## imm = 0x2FE
	jle	LBB8_4
	jmp	LBB8_6
LBB8_2:
	xorl	%r14d, %r14d
	movl	$1, %esi
LBB8_4:
	subl	%r14d, %eax
	.p2align	4, 0x90
LBB8_5:                                 ## =>This Inner Loop Header: Depth=1
	movslq	%esi, %rcx
	movl	(%rbx,%rcx,4), %edx
	leal	378(%rcx), %esi
	movslq	%esi, %rsi
	imulq	$715827883, %rsi, %rsi          ## imm = 0x2AAAAAAB
	movq	%rsi, %rdi
	shrq	$63, %rdi
	shrq	$39, %rsi
	addl	%edi, %esi
	shll	$8, %esi
	leal	(%rsi,%rsi,2), %esi
	negl	%esi
	addl	%ecx, %esi
	addl	$378, %esi                      ## imm = 0x17A
	movslq	%esi, %rsi
	movl	(%rbx,%rsi,4), %edi
	leal	767(%rcx), %r8d
	movslq	%r8d, %r8
	imulq	$715827883, %r8, %r8            ## imm = 0x2AAAAAAB
	movq	%r8, %r9
	shrq	$63, %r9
	shrq	$39, %r8
	addl	%r9d, %r8d
	shll	$8, %r8d
	leal	(%r8,%r8,2), %r8d
	negl	%r8d
	addl	%ecx, %r8d
	addl	$767, %r8d                      ## imm = 0x2FF
	movslq	%r8d, %r8
	xorl	(%rbx,%r8,4), %edx
	xorl	%edi, %edx
	movl	%edx, %r8d
	shrl	$27, %r8d
	xorl	%edx, %r8d
	imull	$1664525, %r8d, %edx            ## imm = 0x19660D
	addl	%edx, %edi
	movl	%edi, (%rbx,%rsi,4)
	addl	%ecx, %edx
	leal	389(%rcx), %esi
	movslq	%esi, %rsi
	imulq	$715827883, %rsi, %rsi          ## imm = 0x2AAAAAAB
	movq	%rsi, %rdi
	shrq	$63, %rdi
	shrq	$39, %rsi
	addl	%edi, %esi
	shll	$8, %esi
	leal	(%rsi,%rsi,2), %esi
	negl	%esi
	addl	%ecx, %esi
	addl	$389, %esi                      ## imm = 0x185
	movslq	%esi, %rsi
	addl	%edx, (%rbx,%rsi,4)
	movl	%edx, (%rbx,%rcx,4)
	leal	1(%rcx), %edx
	movslq	%edx, %rdx
	imulq	$715827883, %rdx, %rdx          ## imm = 0x2AAAAAAB
	movq	%rdx, %rsi
	shrq	$63, %rsi
	shrq	$39, %rdx
	addl	%esi, %edx
	shll	$8, %edx
	leal	(%rdx,%rdx,2), %edx
	negl	%edx
	leal	(%rcx,%rdx), %esi
	incl	%esi
	decl	%eax
	jne	LBB8_5
LBB8_6:
	movl	$768, %eax                      ## imm = 0x300
	.p2align	4, 0x90
LBB8_7:                                 ## =>This Inner Loop Header: Depth=1
	movslq	%esi, %rcx
	leal	378(%rcx), %edx
	movslq	%edx, %rdx
	imulq	$715827883, %rdx, %rdx          ## imm = 0x2AAAAAAB
	movq	%rdx, %rsi
	shrq	$63, %rsi
	shrq	$39, %rdx
	addl	%esi, %edx
	shll	$8, %edx
	leal	(%rdx,%rdx,2), %edx
	negl	%edx
	addl	%ecx, %edx
	addl	$378, %edx                      ## imm = 0x17A
	movslq	%edx, %rdx
	movl	(%rbx,%rdx,4), %esi
	movl	(%rbx,%rcx,4), %edi
	addl	%esi, %edi
	leal	767(%rcx), %r8d
	movslq	%r8d, %r8
	imulq	$715827883, %r8, %r8            ## imm = 0x2AAAAAAB
	movq	%r8, %r9
	shrq	$63, %r9
	shrq	$39, %r8
	addl	%r9d, %r8d
	shll	$8, %r8d
	leal	(%r8,%r8,2), %r8d
	negl	%r8d
	addl	%ecx, %r8d
	addl	$767, %r8d                      ## imm = 0x2FF
	movslq	%r8d, %r8
	addl	(%rbx,%r8,4), %edi
	movl	%edi, %r8d
	shrl	$27, %r8d
	xorl	%edi, %r8d
	imull	$1566083941, %r8d, %edi         ## imm = 0x5D588B65
	xorl	%edi, %esi
	movl	%esi, (%rbx,%rdx,4)
	subl	%ecx, %edi
	leal	389(%rcx), %edx
	movslq	%edx, %rdx
	imulq	$715827883, %rdx, %rdx          ## imm = 0x2AAAAAAB
	movq	%rdx, %rsi
	shrq	$63, %rsi
	shrq	$39, %rdx
	addl	%esi, %edx
	shll	$8, %edx
	leal	(%rdx,%rdx,2), %edx
	negl	%edx
	addl	%ecx, %edx
	addl	$389, %edx                      ## imm = 0x185
	movslq	%edx, %rdx
	xorl	%edi, (%rbx,%rdx,4)
	movl	%edi, (%rbx,%rcx,4)
	leal	1(%rcx), %edx
	movslq	%edx, %rdx
	imulq	$715827883, %rdx, %rdx          ## imm = 0x2AAAAAAB
	movq	%rdx, %rsi
	shrq	$63, %rsi
	shrq	$39, %rdx
	addl	%esi, %edx
	shll	$8, %edx
	leal	(%rdx,%rdx,2), %edx
	negl	%edx
	leal	(%rcx,%rdx), %esi
	incl	%esi
	decl	%eax
	jne	LBB8_7
## %bb.8:
	movl	$28, %eax
	vbroadcastsd	LCPI8_0(%rip), %ymm0    ## ymm0 = [4503599627370495,4503599627370495,4503599627370495,4503599627370495]
	vbroadcastsd	LCPI8_1(%rip), %ymm1    ## ymm1 = [4607182418800017408,4607182418800017408,4607182418800017408,4607182418800017408]
	.p2align	4, 0x90
LBB8_9:                                 ## =>This Inner Loop Header: Depth=1
	vandps	-224(%rbx,%rax,8), %ymm0, %ymm2
	vandps	-192(%rbx,%rax,8), %ymm0, %ymm3
	vandps	-160(%rbx,%rax,8), %ymm0, %ymm4
	vandps	-128(%rbx,%rax,8), %ymm0, %ymm5
	vorps	%ymm1, %ymm2, %ymm2
	vorps	%ymm1, %ymm3, %ymm3
	vorps	%ymm1, %ymm4, %ymm4
	vmovups	%ymm2, -224(%rbx,%rax,8)
	vmovups	%ymm3, -192(%rbx,%rax,8)
	vmovups	%ymm4, -160(%rbx,%rax,8)
	vorps	%ymm1, %ymm5, %ymm2
	vmovups	%ymm2, -128(%rbx,%rax,8)
	cmpq	$380, %rax                      ## imm = 0x17C
	je	LBB8_13
## %bb.10:                              ##   in Loop: Header=BB8_9 Depth=1
	vandps	-96(%rbx,%rax,8), %ymm0, %ymm2
	vandps	-64(%rbx,%rax,8), %ymm0, %ymm3
	vandps	-32(%rbx,%rax,8), %ymm0, %ymm4
	vandps	(%rbx,%rax,8), %ymm0, %ymm5
	vorps	%ymm1, %ymm2, %ymm2
	vorps	%ymm1, %ymm3, %ymm3
	vorps	%ymm1, %ymm4, %ymm4
	vmovups	%ymm2, -96(%rbx,%rax,8)
	vmovups	%ymm3, -64(%rbx,%rax,8)
	vmovups	%ymm4, -32(%rbx,%rax,8)
	vorps	%ymm1, %ymm5, %ymm2
	vmovups	%ymm2, (%rbx,%rax,8)
	addq	$32, %rax
	jmp	LBB8_9
LBB8_13:
	movb	$52, %al
	bzhiq	%rax, 2944(%rbx), %rdx
	movabsq	$4607182418800017408, %rcx      ## imm = 0x3FF0000000000000
	orq	%rcx, %rdx
	movq	%rdx, 2944(%rbx)
	bzhiq	%rax, 2952(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2952(%rbx)
	bzhiq	%rax, 2960(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2960(%rbx)
	bzhiq	%rax, 2968(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2968(%rbx)
	bzhiq	%rax, 2976(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2976(%rbx)
	bzhiq	%rax, 2984(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2984(%rbx)
	bzhiq	%rax, 2992(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 2992(%rbx)
	bzhiq	%rax, 3000(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3000(%rbx)
	bzhiq	%rax, 3008(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3008(%rbx)
	bzhiq	%rax, 3016(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3016(%rbx)
	bzhiq	%rax, 3024(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3024(%rbx)
	bzhiq	%rax, 3032(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3032(%rbx)
	bzhiq	%rax, 3040(%rbx), %rdx
	orq	%rcx, %rdx
	movq	%rdx, 3040(%rbx)
	bzhiq	%rax, 3048(%rbx), %rax
	orq	%rcx, %rax
	movq	%rax, 3048(%rbx)
	movq	3064(%rbx), %rax
	movabsq	$4432916062321256576, %rcx      ## imm = 0x3D84E1AC0DC82880
	andq	3056(%rbx), %rcx
	movl	%eax, %edx
	andl	$1, %edx
	orq	%rcx, %rdx
	movabsq	$1152993127498776576, %rcx      ## imm = 0x1000412401080000
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$32, %rdx
	xorq	%rcx, %rdx
	movq	%rdx, %rcx
	shrq	$16, %rcx
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$8, %rdx
	xorq	%rcx, %rdx
	movq	%rdx, %rcx
	shrq	$4, %rcx
	xorq	%rdx, %rcx
	movq	%rcx, %rdx
	shrq	$2, %rdx
	xorl	%ecx, %edx
	movl	%edx, %ecx
	shrl	%ecx
	xorl	%edx, %ecx
	testb	$1, %cl
	jne	LBB8_15
## %bb.14:
	xorq	$1, %rax
	movq	%rax, 3064(%rbx)
LBB8_15:
	movl	$382, 3072(%rbx)                ## imm = 0x17E
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	vzeroupper
	retq
LBB8_16:
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rcx
	leaq	L_.str.4(%rip), %rdi
	movl	$38, %esi
	movl	$1, %edx
	callq	_fwrite
	movl	$1, %edi
	callq	_exit
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_close1_open2.cold.1
_dsfmt_fill_array_close1_open2.cold.1:  ## @dsfmt_fill_array_close1_open2.cold.1
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_close1_open2(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.2(%rip), %rcx
	movl	$542, %edx                      ## imm = 0x21E
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_close1_open2.cold.2
_dsfmt_fill_array_close1_open2.cold.2:  ## @dsfmt_fill_array_close1_open2.cold.2
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_close1_open2(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.3(%rip), %rcx
	movl	$543, %edx                      ## imm = 0x21F
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_open_close.cold.1
_dsfmt_fill_array_open_close.cold.1:    ## @dsfmt_fill_array_open_close.cold.1
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_open_close(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.2(%rip), %rcx
	movl	$560, %edx                      ## imm = 0x230
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_open_close.cold.2
_dsfmt_fill_array_open_close.cold.2:    ## @dsfmt_fill_array_open_close.cold.2
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_open_close(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.3(%rip), %rcx
	movl	$561, %edx                      ## imm = 0x231
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_close_open.cold.1
_dsfmt_fill_array_close_open.cold.1:    ## @dsfmt_fill_array_close_open.cold.1
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_close_open(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.2(%rip), %rcx
	movl	$578, %edx                      ## imm = 0x242
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_close_open.cold.2
_dsfmt_fill_array_close_open.cold.2:    ## @dsfmt_fill_array_close_open.cold.2
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_close_open(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.3(%rip), %rcx
	movl	$579, %edx                      ## imm = 0x243
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_open_open.cold.1
_dsfmt_fill_array_open_open.cold.1:     ## @dsfmt_fill_array_open_open.cold.1
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_open_open(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.2(%rip), %rcx
	movl	$596, %edx                      ## imm = 0x254
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function dsfmt_fill_array_open_open.cold.2
_dsfmt_fill_array_open_open.cold.2:     ## @dsfmt_fill_array_open_open.cold.2
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	leaq	L___func__.dsfmt_fill_array_open_open(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.3(%rip), %rcx
	movl	$597, %edx                      ## imm = 0x255
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"dSFMT2-19937:117-19:ffafffffffb3f-ffdfffc90fffd"

L___func__.dsfmt_fill_array_close1_open2: ## @__func__.dsfmt_fill_array_close1_open2
	.asciz	"dsfmt_fill_array_close1_open2"

L_.str.1:                               ## @.str.1
	.asciz	"dSFMT.c"

L_.str.2:                               ## @.str.2
	.asciz	"size % 2 == 0"

L_.str.3:                               ## @.str.3
	.asciz	"size >= DSFMT_N64"

L___func__.dsfmt_fill_array_open_close: ## @__func__.dsfmt_fill_array_open_close
	.asciz	"dsfmt_fill_array_open_close"

L___func__.dsfmt_fill_array_close_open: ## @__func__.dsfmt_fill_array_close_open
	.asciz	"dsfmt_fill_array_close_open"

L___func__.dsfmt_fill_array_open_open:  ## @__func__.dsfmt_fill_array_open_open
	.asciz	"dsfmt_fill_array_open_open"

L_.str.4:                               ## @.str.4
	.asciz	"DSFMT_MEXP doesn't match with dSFMT.c\n"

	.comm	_dsfmt_global_data,3088,4       ## @dsfmt_global_data
.subsections_via_symbols
