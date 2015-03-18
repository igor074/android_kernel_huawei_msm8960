#include <asm/div64.h>
#include <linux/reciprocal_div.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> cm-10.0

u32 reciprocal_value(u32 k)
{
	u64 val = (1LL << 32) + (k - 1);
	do_div(val, k);
	return (u32)val;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(reciprocal_value);
>>>>>>> cm-10.0
