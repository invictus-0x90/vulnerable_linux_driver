#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x64ab665b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x72200fa8, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0x17c447fd, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xb5419b40, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xd33a328c, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xb99584d5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

