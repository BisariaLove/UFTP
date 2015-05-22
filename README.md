uftp
====

USC558L => Lab5 Implementing Custom FTP


Entry to add in /etc/hosts
=========================
```
127.0.0.1 sen
127.0.0.1 rev
```

Alias to add in ~/.bashrc or ~/.profile (Shorcuts)
=================================================
```
alias synd='scp -r /home/dan/Work/uftp sc558ce@users.deterlab.net:/proj/USC558L/exp/drehman/work'
alias a='make build_a'
alias b='make build_b'
alias ra='./bin/a etc/data/data.bin rev:etc/data/recv.bin'
alias rb='./bin/b'
```

Run
==
```
// 1. Build node a
>> a
// 2. Build node b
>> b
// 3. Start node b
>> rb
// 4. Start node a
>> ra
```


## STYLEGUIDE

### TODO: Please add / edit to the below code demonstration

*All `//` comments in styleguide below are not actualy style specifications, just comments.*

```
// File: styleguide.c
// Following sections of includes should go from most core to least
// 0. first include should be the file's header file
#include "styleguide.h"

// 1. clib
#include <stdlib.h>
#include <stdio.h>

// 2. external dependencies
#include <evhtp.h>

// 3. other in-project header files
#include "other.h"

// Preprocessor macros and enums should be all uppercase
#define TWENTY 20

// Try to avoid declaring globals -- if they are necessary, try to keep
// them in global_t struct defined in globals.h
//
// static int global_variable;

typedef enum {
  ENUM_NAME__BASE_VALUE = 0,
  ENUM_NAME__OTHER_VALUE,
  ENUM_NAME__YET_ANOTHER_VALUE,
  NUM__ENUM_NAME,
} enum_name_t;

/**
 * @brief Doxygen style comments!
 *
 * @param a Some integer
 * @param b Another integer
 * @return A pointer to 20 bytes of hot, fresh memory
 * @warning You must free the returned pointer
 */
static char *_non_exported_static_func(int a, int b)
{
    // don't put (even short) expressions on same line as if
    // also, put constants first in boolean expressions
    if (3 <= a)
        a = 3;

    // if { } else { } should always have curlies
    if (30 >= b) {
        ++b; // pre- over post-increment operators
        --a;
    } else {
        a = a * b; // space between operators
    }

    // use MALLOC + FREE macros (for eventual[?] repo-wide allocator replacement)
    return MALLOC(20);
}
```


Contributors
===========

Lead
----
* Ramitha
* Danish
* Love

Project
=======
* http://www.isi.edu/~youngcho/csci558l-f14/lab5-f14.pdf

Due
====
* Report : September 13
* Demo   : September 15

Summary
======
This project will build on the last assignment. In it you will explore
the real-world implications
of how TCP performs under less than perfect conditions. Your goal is to
develop a file transfer
program that uses your custom protocol that performs better than any
other program available.
This is your first competitive laboratory that will go head to head
against one other randomly
selected team’s result. We suggest doing the following experiments using
the Ubuntu operating
system.

While TCP is effective, reliable, and relatively robust on the Internet,
it doesn't always give us
the best throughput under every circumstance. In this section you'll
design an IP based file-
transfer utility. The design and implementation of the utility is up to
your group, however it must
full-fill only three requirements: it must use IP (so it can be routed),
it must transfer the file
reliably (with no errors) and it must be implemented with a command-line
interface similar to
scp.

The link speed between the sender and receiver must be 100Mbps and the
test file size must be at
least 1GBytes. You should emulate the delay and the loss rate of the
link using the delay node.
You should test your system under various different conditions. However
two settings that you
must expose your system for the assignment are:

* The Delay (RTT) of 10ms with the Loss rate of 1%
* The Delay (RTT) of 200ms with the Loss rate of 20%

For your demo and competition, you will need to create an experiment
with two nodes connected
by the second link (delay of 200ms and the loss rate of 20%) in both
direction defined in your .ns
file.

You will also need to create a 1GB file in your local /tmp directory of
the sender computer.
Your program will need to reliably transfer the entire file from the
sender to the receiver then
back to the sender at the highest performance possible; all through this
bad network link.

You must also create a script that will timestamp right before the
execution of the program and
timestamp at the end of the successful transference of the file. The
total time for the file to make
the round trip will be used to compare your program’s performance
against other team’s program
performance. You also will need to prove the reliability of your system
by running MD5 on both
the original file and the received file.

The execution of the system should be straight forward and any notion of
cheating will result in
automatic defeat in the competition and possibly zero on the lab score.
The goal of this task is to
encourage a healthy competitive development environment for everyone.
You are encouraged to
help each other to get the best result. However, at the end, the faster
team will win.


Daily activities
================

* As a part of the assignment, each group must post the progress and
the result of your system
on the Laboratory 5 Results and Discussion forum EVERY SINGLE DAY.
* The minimum transfer rate for your final FTP is 20 Mbps to compete
against the other team.
* Describe, in detail, the concept(s) behind your file transfer
utility, results, and the analysis in
the document that must be submitted on September 13 th by 11:55pm. You
also must
demonstrate your system to your T.A. during his office hour on Sept
15th.
* The scoring will be based on the submitted document and T.A.s
verification of the system.
