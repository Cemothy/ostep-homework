# Questions #

- `-l`
  - Anzahl der Elemente, die jeder Produzent erzeugt.
- `-m`
  - Größe des gemeinsamen Puffers für Produzenten und Konsumenten.
- `-p`
  - Anzahl der Produzenten.
- `-c`
  - Anzahl der Konsumenten.
- `-P`
  - Schlaf-String für jeden Produzenten.
- `-C`
  - Schlaf-String für jeden Konsumenten.
- `-v`
  - Verbose-Flag: Verfolgt und gibt aus, was passiert.
- `-t`
  - Timing-Flag: Misst und gibt die gesamte Ausführungszeit aus.


1. Our first question focuses on main-two-cvs-while.c (the working solution). First, study the code. Do you think you have an understanding of what should happen when you run the program?

    This is the working solution. 
    -Using while
    -Producer waits on empty and signals on fill
    -Consumer waits on fill and signals on empty


2. Run with one producer and one consumer, and have the producer
produce a few values. Start with a buffer (size 1), and then increase
it. How does the behavior of the code change with larger buffers?
(or does it?) What would you predict num full to be with different
buffer sizes (e.g., -m 10) and different numbers of produced items
(e.g., -l 100), when you change the consumer sleep string from
default (no sleep) to -C 0,0,0,0,0,0,1?

    With buffer size 1: 
        Producer and Cunsomer switch between each other

    With buffer size >1: 
        Producer and Customer still switch between each other but dont necessarily have to!

    Dont know what is meant with num full?? 

    The sleep String: 
        Makes it so that the Producer will first Produce all products then the Consumer will consume all



3. If possible, run the code on different systems (e.g., a Mac and Linux).
Do you see different behavior across these systems?

    Dont have that possibility 

4. Let’s look at some timings. How long do you think the following execution, with one producer, three consumers, a single-entry
shared buffer, and each consumer pausing at point c3 for a second, will take? ./main-two-cvs-while -p 1 -c 3 -m 1 -C
0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v
-t

    It will take about 10 seconds (+EOS), because the producer always has to wait for a consumer to empty the buffer which takes one second (because of sleep). 
    But because the Producer sleeps at c3, where it holds the lock, the other consumers cant do work in parallel!
    So every second one is produced and consumed!
    -> 12 seconds 

5. Now change the size of the shared buffer to 3 (-m 3). Will this make
any difference in the total time?

    It doesnt do a difference, because at point c3 the thread is holding the lock preventing any further progress!
    -> 11 seconds

6. Now change the location of the sleep to c6 (this models a consumer taking something off the queue and then doing something
with it), again using a single-entry buffer. What time do you predict in this case? ./main-two-cvs-while -p 1 -c 3 -m 1
-C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10
-v -t

    This takes about 5 Seconds. 
    Because when one Consumer is sleeping another can take over the work since no lock is held by the sleeping consumer!
    The Distrubution is way more balanced

7. Finally, change the buffer size to 3 again (-m 3). What time do you
predict now?

    Again 5 seconds.
    This just changes the order of producing and consuming.

8. Now let’s look at main-one-cv-while.c. Can you configure
a sleep string, assuming a single producer, one consumer, and a
buffer of size 1, to cause a problem with this code?

    No.


9. Now change the number of consumers to two. Can you construct
sleep strings for the producer and the consumers so as to cause a
problem in the code?

    -P 0,0,0,0,0,0,1
    Also there is just pure chance 
    When both consumers are sleeping and a producer fills the buffer waking up an consumer, this one will consume and then wake up another thread. 
    If he wakes up the producer everything is fine but if he wakes up the other consumer we will have a deadlock!

    Cant do this since i cannot say which one has to be woken up --> cannot pause at p3, because this will only pause after woken up! And p2 will pause before putting to sleep



10. Now examine main-two-cvs-if.c. Can you cause a problem to
happen in this code? Again consider the case where there is only
one consumer, and then the case where there is more than one.

    One consumer is fine. 
    Two consumer there can be a case where on steals the item.

    ./main-two-cvs-if -p 1 -c 2 -m 1 -P 1,0,0,0,0,0,0 -C 1,0,0,0,0,0,0:0,0,0,0,3,0,0 -l 10 -v -t


11. Finally, examine main-two-cvs-while-extra-unlock.c. What
problem arises when you release the lock before doing a put or a
get? Can you reliably cause such a problem to happen, given the
sleep strings? What bad thing can happen?

    Well not with the way this is programmed.
    Producer ran, put an Item on the Buffer and waits on the condition
    Consumer 1 runs, sees there is an Item on the Buffer so it goes past the while loop and unlocks the Lock, now there is an Interrupt
    Consumer 2 runs, also sees there is an Item on the Buffer, goest past the while loop, unlocks, and takes off the Item. Interrupt
    Consumer 1 runs again, also taking off an Item but there is none left since C1 already took it. 

    This would be possible to simulate if there was a sleep point between p3 and p4 (after unlock, before consume)

    
