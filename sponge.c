# include "stdio.h"
# include "fcntl.h"
# include "unistd.h"
# include "string.h"
# include "stdlib.h"

# define BUFF_LEN 101

typedef struct lst
{
	char buff[BUFF_LEN];
	int  size;
	struct lst *next;
}		lst;

void Spongify(lst *elem)
{
	int i;
	int s;

	i = 0;
	s = 0;
	while (i < elem->size - 1)
	{
		if (elem->buff[i] != ' ')
			elem->buff[i] = elem->buff[i] - (32 * s);
		s ^= 1;
		i++;
	}
}

int main(int argc, char **argv)
{
	int fd;
	int bytes;
	lst *head;
	lst *elem;
	char buffer[BUFF_LEN];

	if (argc > 3)
	{
		printf("Usage: ./sponge [file] or type text\n");
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	head = calloc(1, sizeof(*head));
	elem = head;
	while((bytes = read(((fd != -1) * fd), buffer, BUFF_LEN - 1)) != 0)
	{
		buffer[bytes] = '\0';
		memcpy(elem->buff, buffer, bytes);
		elem->size = bytes;
		elem->next = calloc(1, sizeof(*elem));
	}

	fd = ((argc == 3) ? (open(argv[2], O_WRONLY | O_CREAT, 0777)) : (0));
	printf("Fd %d\n", fd);
	while (head)
	{
		Spongify(head);
		write(fd, head->buff, head->size);
		head = head->next;
	}
	exit(0);
}
