Questions


1. With a linear page table, you need a single register to locate the
page table, assuming that hardware does the lookup upon a TLB
miss. How many registers do you need to locate a two-level page
table? A three-level table?

    As many as registers as yoú have levels.


2. Use the simulator to perform translations given random seeds 0,
1, and 2, and check your answers using the -c flag. How many
memory references are needed to perform each lookup?

    Level + 1, the first two are to access the PageDircetoryEntry / PageTabeEntry and the third is the memory access itself.


3. Given your understanding of how cache memory works, how do
you think memory references to the page table will behave in the
cache? Will they lead to lots of cache hits (and thus fast accesses?)
Or lots of misses (and thus slow accesses)?

    Cache hits should be the same as in Linear. The only difference is when it does miss you will need one more memory access!