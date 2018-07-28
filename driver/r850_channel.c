// r850_channel.h

#include "print_format.h"

#include <linux/errno.h>
#include <linux/string.h>

#include "r850_channel.h"

static const u8 ch_regs[][R850_NUM_REGS - 0x08] = {
	// UHF 13ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe0, 0x00, 0x30,
		0x9f, 0xbb, 0xb8, 0xb0, 0xd2, 0x00, 0xcd, 0x55,
		0xa7, 0x40, 0x89, 0x90, 0x16, 0x2e, 0x09, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0xa9, 0x6c,
		0x53, 0xab, 0x5b, 0x46, 0xb3, 0x97, 0x7e, 0x45
	},
	// UHF 14ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x9f, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0xcb, 0x58, 0xb8, 0x0b, 0x23,
		0x21, 0xf1, 0x0f, 0x5f, 0xc4, 0x20, 0xa9, 0x8c,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 15ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x90, 0x16, 0x2e, 0x0b, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0xa9, 0x8c,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 16ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x91, 0x16, 0x2e, 0x08, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0xa9, 0x8c,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 17ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x91, 0x16, 0x2e, 0x09, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 18ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x91, 0x16, 0x2e, 0x0a, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 19ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x91, 0x16, 0x2e, 0x0b, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 20ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x92, 0x16, 0x2e, 0x08, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 21ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x92, 0x16, 0x2e, 0x09, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 22ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x92, 0x16, 0x2e, 0x0a, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 23ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x92, 0x16, 0x2e, 0x0b, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 24ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x93, 0x16, 0x2e, 0x08, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 25ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x93, 0x16, 0x2e, 0x09, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 26ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x97, 0xbb, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x93, 0x16, 0x2e, 0x0a, 0x23,
		0x21, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x57, 0x7e, 0x45
	},
	// UHF 27ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8f, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x17, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 28ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8f, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x97, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 29ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8f, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x17, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 30ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8f, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x97, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 31ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8f, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x17, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 32ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x88, 0x0c, 0x97, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 33ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x17, 0x04, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 34ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x97, 0x04, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 35ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x17, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 36ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x97, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 37ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x17, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 38ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x97, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 39ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x17, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 40ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x89, 0x0c, 0x97, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 41ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x17, 0x04, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 42ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x97, 0x04, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 43ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x17, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 44ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x89, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x97, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 45ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x17, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 46ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x97, 0x06, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 47ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x17, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 48ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8a, 0x0c, 0x97, 0x07, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
	// UHF 49ch
	{
		0xca, 0xc0, 0x32, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8b, 0x0c, 0x17, 0x04, 0x22,
		0x00, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x45, 0x73, 0x43, 0x7d, 0x45
	},
	// UHF 50ch
	{
		0xca, 0xc0, 0x32, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8b, 0x0c, 0x97, 0x04, 0x22,
		0x00, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x45, 0x73, 0x43, 0x7d, 0x45
	},
	// UHF 51ch
	{
		0xca, 0xc0, 0x32, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x8e, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0xc1, 0x8b, 0x0c, 0x17, 0x05, 0x22,
		0x00, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5b,
		0x53, 0x99, 0x6b, 0x45, 0x73, 0x43, 0x79, 0x45
	},
	// UHF 52ch
	{
		0xca, 0xc0, 0x72, 0x50, 0x00, 0xe4, 0x00, 0x30,
		0x86, 0x3b, 0xb8, 0xb7, 0xd2, 0x10, 0xcd, 0x55,
		0xa7, 0x42, 0x81, 0x8b, 0x0c, 0x97, 0x05, 0x23,
		0x20, 0xf1, 0x0c, 0x5f, 0xc4, 0x20, 0x9a, 0x5a,
		0xc1, 0x99, 0x6b, 0x44, 0x53, 0x53, 0x7d, 0x45
	},
};

int r850_channel_get_regs(u32 no, u8 *regs)
{
	int ret = 0;

	if (no < 63)
		ret = -EINVAL;

	no -= 63;

	if (no < 40)
		memcpy(regs, &ch_regs[no], R850_NUM_REGS - 0x08);
	else
		ret = -ENOSYS;

	return ret;
}