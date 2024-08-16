#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <assert.h>

// always 16 bytes argument
struct ioctl_arg_u
{
	uint8_t		b0;
	uint8_t		b1;
	uint8_t		b2;
	uint8_t 	b3;
	uint32_t	i0;
	uint64_t	uaddr;
};

static inline int create_head_node(int fd, uint8_t head_index)
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index;
	return ioctl(fd, 0x133a, &arg);
}

static inline int destory_head(int fd, uint8_t head_index)
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index;
	return ioctl(fd, 0x133b, &arg);
}

static inline int consume_head_data(int fd, uint8_t head_index, uint8_t data_key, uint8_t buf[])
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index;
	arg.b3 = data_key;
	arg.uaddr = buf;
	return ioctl(fd, 0x133c, &arg);
}

static inline int do_clone(int fd, uint8_t head_index1, uint8_t head_index2, uint8_t data_key)
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index1;
	arg.b1 = head_index2;
	arg.b3 = data_key;
	return ioctl(fd, 0x1339, &arg);
}

static inline int remove_data_node(int fd, uint8_t head_index, uint8_t data_key)
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index;
	arg.b3 = data_key;
	return ioctl(fd, 0x1338, &arg);
}

static inline int create_data_node(int fd, uint8_t head_index, uint16_t key, uint8_t buf[])
{
	struct ioctl_arg_u arg = {0};
	arg.b0 = head_index;
	// XXX
	*(uint16_t*)&(arg.b2) = key;
	arg.uaddr = buf;
	return ioctl(fd, 0x1337, &arg);
}

static inline void hexdump(char *data, uint32_t size)
{
	for(int i = 0; i < size; i++)
	{
		if (i && (i % 16 == 0))
			putchar('\n');
		
		printf("%02x ", data[i] & 0xff);
	}
	putchar('\n');
}