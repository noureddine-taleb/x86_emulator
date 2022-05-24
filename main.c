#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "instruction.h"
#include "cpu.h"
#include "init.h"
#include "memory.h"
#include "parser.h"

int main(int argc, char **argv)
{
	int fd;

	if (argc < 2)
		die("help: %s <bin>\n", argv[0]);

	if ((fd = open(argv[1], O_RDONLY)) < 0)
		die("failed to open the file: %s\n", argv[1]);

	if (load_image(fd, 0) < 0)
	{
		perror("can't load image");
		die("");
	}

	set_eip(0);

	init_instructions();

	while (1)
	{
		bool mode32;
		u8 prefix;
		bool chsz_op, chsz_ad;

		mode32 = is_mode32();
		prefix = parse_prefix();
		chsz_op = prefix & CHSZ_OP;
		chsz_ad = prefix & CHSZ_AD;

		if (mode32 ^ chsz_op)
		{
			set_chsz_ad(!(mode32 ^ chsz_ad));
			parse();
			exec();
		}
		else
		{
			set_chsz_ad(mode32 ^ chsz_ad);
			parse();
			exec();
		}
	}
}