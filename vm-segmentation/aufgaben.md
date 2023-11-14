# Questions 

1. First let’s use a tiny address space to translate some addresses. Here’s
a simple set of parameters with a few different random seeds; can
you translate the addresses?

a = address space size
p = physical memory size
b = b0 = value of base register 0
B = b1 = value of base register 1
l = l0 = value of limit refister 0
L = l1 = value of limit register 1



segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512
-L 20 -s 0

    ARG seed 0
    ARG address space size 128
    ARG phys mem size 512

    Segment register information:

    Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
    Segment 0 limit                  : 20

    Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
    Segment 1 limit                  : 20

    Virtual Address Trace
    VA  0: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
    VA  1: 0x00000061 (decimal:   97) --> SEGMENTATION VIOLATION (SEG1)
    VA  2: 0x00000035 (decimal:   53) --> SEGMENTATION VIOLATION (SEG0)
    VA  3: 0x00000021 (decimal:   33) --> SEGMENTATION VIOLATION (SEG0)
    VA  4: 0x00000041 (decimal:   65) --> SEGMENTATION VIOLATION (SEG1)



segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512
-L 20 -s 1

ARG seed 1
ARG address space size 128
ARG phys mem size 512

    Segment register information:

    Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
    Segment 0 limit                  : 20 -> valid 0 bis 20

    Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
    Segment 1 limit                  : 20 -> valid (128-20) bis 128

    Virtual Address Trace
    VA  0: 0x00000011 (decimal:   17) --> VALID in SEG0: 0x00000011 (decimal:   17) -> base seg0 + offset
    VA  1: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492) -> base seg0 + offset (=size - maxsize)
    VA  2: 0x00000061 (decimal:   97) --> SEGMENTATION VIOLATION (SEG1)
    VA  3: 0x00000020 (decimal:   32) --> SEGMENTATION VIOLATION (SEG0)
    VA  4: 0x0000003f (decimal:   63) --> SEGMENTATION VIOLATION (SEG0)


segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512
-L 20 -s 2

    ARG seed 2
    ARG address space size 128
    ARG phys mem size 512

    Segment register information:

    Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
    Segment 0 limit                  : 20

    Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
    Segment 1 limit                  : 20

    Virtual Address Trace
    VA  0: 0x0000007a (decimal:  122) --> VALID in SEG1: 0x000001fa (decimal:  506)
    VA  1: 0x00000079 (decimal:  121) --> VALID in SEG1: 0x000001f9 (decimal:  505)
    VA  2: 0x00000007 (decimal:    7) --> VALID in SEG0: 0x00000007 (decimal:    7)
    VA  3: 0x0000000a (decimal:   10) --> VALID in SEG0: 0x0000000a (decimal:   10)
    VA  4: 0x0000006a (decimal:  106) --> SEGMENTATION VIOLATION (SEG1)





2. Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is
the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and
highest illegal addresses in this entire address space? Finally, how
would you run segmentation.py with the -A flag to test if you
are right?
    
    Seg0: 0 bis 19
    Seg1: 108 bis 127

    VA  0: 0x00000000 (decimal:    0) --> VALID in SEG0: 0x00000000 (decimal:    0)
    VA  1: 0x00000013 (decimal:   19) --> VALID in SEG0: 0x00000013 (decimal:   19)
    VA  2: 0x00000014 (decimal:   20) --> SEGMENTATION VIOLATION (SEG0)
    VA  3: 0x0000006b (decimal:  107) --> SEGMENTATION VIOLATION (SEG1)
    VA  4: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
    VA  5: 0x0000007f (decimal:  127) --> VALID in SEG1: 0x000001ff (decimal:  511)
    Error: virtual address 128 cannot be generated in an address space of size 128


3. Let’s say we have a tiny 16-byte address space in a 128-byte physical
memory. What base and bounds would you set up so as to get
the simulator to generate the following translation results for the
specified address stream: valid, valid, violation, ..., violation, valid,
valid? Assume the following parameters:
segmentation.py -a 16 -p 128
-A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
--b0 ? --l0 ? --b1 ? --l1 ?


python segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 15 --l1 2 -c
ARG seed 0
ARG address space size 16
ARG phys mem size 128

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 2

  Segment 1 base  (grows negative) : 0x0000000f (decimal 15)
  Segment 1 limit                  : 2

Virtual Address Trace
  VA  0: 0x00000000 (decimal:    0) --> VALID in SEG0: 0x00000000 (decimal:    0)
  VA  1: 0x00000001 (decimal:    1) --> VALID in SEG0: 0x00000001 (decimal:    1)
  VA  2: 0x00000002 (decimal:    2) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000003 (decimal:    3) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x00000004 (decimal:    4) --> SEGMENTATION VIOLATION (SEG0)
  VA  5: 0x00000005 (decimal:    5) --> SEGMENTATION VIOLATION (SEG0)
  VA  6: 0x00000006 (decimal:    6) --> SEGMENTATION VIOLATION (SEG0)
  VA  7: 0x00000007 (decimal:    7) --> SEGMENTATION VIOLATION (SEG0)
  VA  8: 0x00000008 (decimal:    8) --> SEGMENTATION VIOLATION (SEG1)
  VA  9: 0x00000009 (decimal:    9) --> SEGMENTATION VIOLATION (SEG1)
  VA 10: 0x0000000a (decimal:   10) --> SEGMENTATION VIOLATION (SEG1)
  VA 11: 0x0000000b (decimal:   11) --> SEGMENTATION VIOLATION (SEG1)
  VA 12: 0x0000000c (decimal:   12) --> SEGMENTATION VIOLATION (SEG1)
  VA 13: 0x0000000d (decimal:   13) --> SEGMENTATION VIOLATION (SEG1)
  VA 14: 0x0000000e (decimal:   14) --> VALID in SEG1: 0x0000000d (decimal:   13)
  VA 15: 0x0000000f (decimal:   15) --> VALID in SEG1: 0x0000000e (decimal:   14)

  Wobei hier -b0 & b1 irrelevant sind solange diese innerhalb der physical Memory liegen

4. Assume we want to generate a problem where roughly 90% of the
randomly-generated virtual addresses are valid (not segmentation
violations). How should you configure the simulator to do so?
Which parameters are important to getting this outcome?

    Es sollte 90% des Address spaces abgedeckt sein von den Segmenten, das heißt -> l0 + l1 / a = 0,9 
    In unserem Beispiel oben wäre a = 16 und somit l0 = l2 = 7 -> 14/16 = 0,875

    PS C:\Users\Robin\OneDrive\Desktop\HTWG\5.Semester\BSYS\ostep-github\ostep-homework\vm-segmentation> python segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 7 --b1 15 --l1 7 -c
    ARG seed 0
    ARG address space size 16
    ARG phys mem size 128

    Segment register information:

    Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
    Segment 0 limit                  : 7

    Segment 1 base  (grows negative) : 0x0000000f (decimal 15)
    Segment 1 limit                  : 7

    Virtual Address Trace
    VA  0: 0x00000000 (decimal:    0) --> VALID in SEG0: 0x00000000 (decimal:    0)
    VA  1: 0x00000001 (decimal:    1) --> VALID in SEG0: 0x00000001 (decimal:    1)
    VA  2: 0x00000002 (decimal:    2) --> VALID in SEG0: 0x00000002 (decimal:    2)
    VA  3: 0x00000003 (decimal:    3) --> VALID in SEG0: 0x00000003 (decimal:    3)
    VA  4: 0x00000004 (decimal:    4) --> VALID in SEG0: 0x00000004 (decimal:    4)
    VA  5: 0x00000005 (decimal:    5) --> VALID in SEG0: 0x00000005 (decimal:    5)
    VA  6: 0x00000006 (decimal:    6) --> VALID in SEG0: 0x00000006 (decimal:    6)
    VA  7: 0x00000007 (decimal:    7) --> SEGMENTATION VIOLATION (SEG0)
    VA  8: 0x00000008 (decimal:    8) --> SEGMENTATION VIOLATION (SEG1)
    VA  9: 0x00000009 (decimal:    9) --> VALID in SEG1: 0x00000008 (decimal:    8)
    VA 10: 0x0000000a (decimal:   10) --> VALID in SEG1: 0x00000009 (decimal:    9)
    VA 11: 0x0000000b (decimal:   11) --> VALID in SEG1: 0x0000000a (decimal:   10)
    VA 12: 0x0000000c (decimal:   12) --> VALID in SEG1: 0x0000000b (decimal:   11)
    VA 13: 0x0000000d (decimal:   13) --> VALID in SEG1: 0x0000000c (decimal:   12)
    VA 14: 0x0000000e (decimal:   14) --> VALID in SEG1: 0x0000000d (decimal:   13)
    VA 15: 0x0000000f (decimal:   15) --> VALID in SEG1: 0x0000000e (decimal:   14)


5. Can you run the simulator such that no virtual addresses are valid?
How?

    l0 = l1 = 0

    PS C:\Users\Robin\OneDrive\Desktop\HTWG\5.Semester\BSYS\ostep-github\ostep-homework\vm-segmentation> python segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 0 --b1 15 --l1 0 -c
    ARG seed 0
    ARG address space size 16
    ARG phys mem size 128

    Segment register information:

    Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
    Segment 0 limit                  : 0

    Segment 1 base  (grows negative) : 0x0000000f (decimal 15)
    Segment 1 limit                  : 0

    Virtual Address Trace
    VA  0: 0x00000000 (decimal:    0) --> SEGMENTATION VIOLATION (SEG0)
    VA  1: 0x00000001 (decimal:    1) --> SEGMENTATION VIOLATION (SEG0)
    VA  2: 0x00000002 (decimal:    2) --> SEGMENTATION VIOLATION (SEG0)
    VA  3: 0x00000003 (decimal:    3) --> SEGMENTATION VIOLATION (SEG0)
    VA  4: 0x00000004 (decimal:    4) --> SEGMENTATION VIOLATION (SEG0)
    VA  5: 0x00000005 (decimal:    5) --> SEGMENTATION VIOLATION (SEG0)
    VA  6: 0x00000006 (decimal:    6) --> SEGMENTATION VIOLATION (SEG0)
    VA  7: 0x00000007 (decimal:    7) --> SEGMENTATION VIOLATION (SEG0)
    VA  8: 0x00000008 (decimal:    8) --> SEGMENTATION VIOLATION (SEG1)
    VA  9: 0x00000009 (decimal:    9) --> SEGMENTATION VIOLATION (SEG1)
    VA 10: 0x0000000a (decimal:   10) --> SEGMENTATION VIOLATION (SEG1)
    VA 11: 0x0000000b (decimal:   11) --> SEGMENTATION VIOLATION (SEG1)
    VA 12: 0x0000000c (decimal:   12) --> SEGMENTATION VIOLATION (SEG1)
    VA 13: 0x0000000d (decimal:   13) --> SEGMENTATION VIOLATION (SEG1)
    VA 14: 0x0000000e (decimal:   14) --> SEGMENTATION VIOLATION (SEG1)
    VA 15: 0x0000000f (decimal:   15) --> SEGMENTATION VIOLATION (SEG1)