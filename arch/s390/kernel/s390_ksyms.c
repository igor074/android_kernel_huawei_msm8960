#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/kvm_host.h>
>>>>>>> cm-10.0
#include <asm/ftrace.h>

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(_mcount);
#endif
<<<<<<< HEAD
=======
#if defined(CONFIG_KVM) || defined(CONFIG_KVM_MODULE)
EXPORT_SYMBOL(sie64a);
#endif
>>>>>>> cm-10.0
