# LLVM IR is a compiler IR - SHINING的博客 - CSDN博客
2013年12月26日 17:38:17[snsn1984](https://me.csdn.net/snsn1984)阅读数：2447
In this email, I argue that LLVM IR is a poor system for building a
Platform, by which I mean any system where LLVM IR would be a
format in which programs are stored or transmitted for subsequent
use on multiple underlying architectures.
LLVM IR initially seems like it would work well here. I myself was
once attracted to this idea. I was even motivated to put a bunch of
my own personal time into making some of LLVM's optimization passes
more robust in the absence of TargetData a while ago, even with no
specific project in mind. There are several things still missing,
but one could easily imagine that this is just a matter of people
writing some more code.
However, there are several ways in which LLVM IR differs from actual
platforms, both high-level VMs like Java or .NET and actual low-level
ISAs like x86 or ARM.
First, the boundaries of what capabilities LLVM provides are nebulous.
LLVM IR contains:
 * Explicitly Target-specific features. These aren't secret;
   x86_fp80's reason for being is pretty clear.
 * Target-specific ABI code. In order to interoperate with native
   C ABIs, LLVM requires front-ends to emit target-specific IR.
   Pretty much everyone around here has run into this.
 * Implicitly Target-specific features. The most obvious examples of
   these are all the different Linkage kinds. These are all basically
   just gateways to features in real linkers, and real linkers vary
   quite a lot. LLVM has its own IR-level Linker, but it doesn't
   do all the stuff that native linkers do.
 * Target-specific limitations in seemingly portable features.
   How big can the alignment be on an alloca? Or a GlobalVariable?
   What's the widest supported integer type? LLVM's various backends
   all have different answers to questions like these.
Even ignoring the fact that the quality of the backends in the
LLVM source tree varies widely, the question of "What can LLVM IR do?"
has numerous backend-specific facets. This can be problematic for
producers as well as consumers.
Second, and more fundamentally, LLVM IR is a fundamentally
vague language. It has:
 * Undefined Behavior. LLVM is, at its heart, a C compiler, and
   Undefined Behavior is one of its cornerstones.
   High-level VMs typically raise predictable exceptions when they
   encounter program errors. Physical machines typically document
   their behavior very extensively. LLVM is fundamentally different
   from both: it presents a bunch of rules to follow and then offers
   no description of what happens if you break them.
   LLVM's optimizers are built on the assumption that the rules
   are never broken, so when rules do get broken, the code just
   goes off the rails and runs into whatever happens to be in
   the way. Sometimes it crashes loudly. Sometimes it silently
   corrupts data and keeps running.
   There are some tools that can help locate violations of the
   rules. Valgrind is a very useful tool. But they can't find
   everything. There are even some kinds of undefined behavior that
   I've never heard anyone even propose a method of detection for.
 * Intentional vagueness. There is a strong preference for defining
   LLVM IR semantics intuitively rather than formally. This is quite
   practical; formalizing a language is a lot of work, it reduces
   future flexibility, and it tends to draw attention to troublesome
   edge cases which could otherwise be largely ignored.
   I've done work to try to formalize parts of LLVM IR, and the
   results have been largely fruitless. I got bogged down in
   edge cases that no one is interested in fixing.
 * Floating-point arithmetic is not always consistent. Some backends
   don't fully implement IEEE-754 arithmetic rules even without
   -ffast-math and friends, to get better performance.
If you're familiar with "write once, debug everywhere" in Java,
consider the situation in LLVM IR, which is fundamentally opposed
to even trying to provide that level of consistency. And if you allow
the optimizer to do subtarget-specific optimizations, you increase
the chances that some bit of undefined behavior or vagueness will be
exposed.
Third, LLVM is a low level system that doesn't represent high-level
abstractions natively. It forces them to be chopped up into lots of
small low-level instructions.
 * It makes LLVM's Interpreter really slow. The amount of work
   performed by each instruction is relatively small, so the interpreter
   has to execute a relatively large number of instructions to do simple
   tasks, such as virtual method calls. Languages built for interpretation
   do more with fewer instructions, and have lower per-instruction
   overhead.
 * Similarly, it makes really-fast JITing hard. LLVM is fast compared
   to some other static C compilers, but it's not fast compared to
   real JIT compilers. Compiling one LLVM IR level instruction at a
   time can be relatively simple, ignoring the weird stuff, but this
   approach generates comically bad code. Fixing this requires
   recognizing patterns in groups of instructions, and then emitting
   code for the patterns. This works, but it's more involved.
 * Lowering high-level language features into low-level code locks
   in implementation details. This is less severe in native code,
   because a compiled blob is limited to a single hardware platform
   as well. But a platform which advertizes architecture independence
   which still has all the ABI lock-in of HLL implementation details
   presents a much more frightening backwards compatibility specter.
 * Apple has some LLVM IR transformations for Objective-C, however
   the transformations have to reverse-engineer the high-level semantics
   out of the lowered code, which is awkward. Further, they're
   reasoning about high-level semantics in a way that isn't guaranteed
   to be safe by LLVM IR rules alone. It works for the kinds of code
   clang generates for Objective C, but it wouldn't necessarily be
   correct if run on code produced by other front-ends. LLVM IR
   isn't capable of representing the necessary semantics for this
   unless we start embedding Objective C into it.
In conclusion, consider the task of writing an independent implementation
of an LLVM IR Platform. The set of capabilities it provides depends on who
you talk to. Semantic details are left to chance. There are features
which require a bunch of complicated infrastructure to implement which
are rarely used. And if you want light-weight execution, you'll
probably need to translate it into something else better suited for it
first. This all doesn't sound very appealing.
LLVM isn't actually a virtual machine. It's widely acknoledged that the
name "LLVM" is a historical artifact which doesn't reliably connote what
LLVM actually grew to be. LLVM IR is a compiler IR.
Dan
