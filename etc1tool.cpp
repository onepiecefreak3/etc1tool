#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h> 
#include <iostream> 
#include <iomanip>
#include "rg_etc1.h"
using namespace std;

//requires rg_etc1.cpp and rg_etc1.h from https://code.google.com/p/rg-etc1/source/browse/#svn%2Ftrunk

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

u64 switchEndianness(u64 v)
{
	u64 ret;
	u8* retA = (u8*)&ret;
	u8* vA = (u8*)&v;

	retA[0] = vA[7];
	retA[1] = vA[6];
	retA[2] = vA[5];
	retA[3] = vA[4];
	retA[4] = vA[3];
	retA[5] = vA[2];
	retA[6] = vA[1];
	retA[7] = vA[0];

	return ret;
}

int main(int argc, char* argv[]) {
	//etc1tool by onepiecefreak | Based on script from smealum | Used rg_etc1 from richgel999

	if (argc < 2) { cout << "Type \"etc1tool.exe -help\" into the command line for further explanation and help!"; return 0; }
	char mode[10]; char mode1[] = "-d"; char mode2[] = "-c"; char mode3[] = "-help";
	sscanf(argv[1], "%s", &mode);
	if (strcmp(mode, mode1) != 0 && strcmp(mode, mode2) != 0 && strcmp(mode, mode3) != 0) { cout << "Unknown mode!\nType \"etc1tool.exe -help\" into the command line for further explanation and help!"; return 0; }

	if (strcmp(mode, mode3) == 0) {
		cout << "This tool converts etc1/etc1a4 converted pictures into raw pixels data or vice versa.\nThe raw pixels data contains RGBA values for each pixel in the picture.\n";
		cout << "The etc1a4 data contains 8 byte of Alpha value data and 8 byte of RGB pixel data per every 4x4 pixel grid.\n\n";
		cout << "This tool is controlled through the command line and uses 3 different modes:\n-c	To compress mentioned raw pixels data into etc1a4 data\n";
		cout << "-d	To decompress mentioned etc1/etc1a4 data into raw pixels data\n-help	To show this help text and explanation\n\n";
		cout << "To use this tool properly, you have to type the following into the command line:\netc1tool.exe <mode> <input> <output> <width> <height> <alpha>\n";
		cout << "<mode>		One of the modes mentioned above\n<input>		Filepath of the file from which the data should be converted\n<output>	Filepath of the file to which the converted data should be placed\n";
		cout << "<width>		The width of the original picture\n<height>	The height of the original picture\n<alpha>		The Alphamode, which should be used\n\n";
		cout << "There are 2 different Alphamodes:\n0	The picture will be converted with no Alphavalues preserved\n1	The picture will be converted with Alphavalues preserved";
		return 0;
	}

	if (argc < 7) { cout << "Too few arguments!\nType \"etc1tool.exe -help\" into the command line for further explanation and help!"; return 0; }

	int w, h, alpha;
	FILE* f = fopen(argv[2], "rb");
	sscanf(argv[4], "%d", &w);
	sscanf(argv[5], "%d", &h);
	sscanf(argv[6], "%d", &alpha);

	if (!f) { cout << "Couldn't open file " << argv[2] << "!\nPlease check the path and name of the file!";  return 0; }

	if (strcmp(mode, mode1) == 0) {
		if (alpha == 0) {
			u64 block;
			u32 pixels[4 * 4];
			int i, j, n, x, y, count;
			float percent;
			u32* ptr;
			u32* buffer = (u32*)malloc(sizeof(u32)*w*h * 4);
			count = 0;

			n = (w / 4) * (h / 4);
			cout << fixed << setprecision(2) << "Decompressing... 0/" << n << " blocks | 0.00%";

			for (j = 0; j < h / 8; j++) {
				for (i = 0; i < w / 8; i++) {
					//first ETC1-Block
						//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 0; x < 4; x++)
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] = *(ptr++);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//second ETC1-Block
						//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 0; x < 4; x++)
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] = *(ptr++);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//third ETC1-Block
						//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 4; x < 8; x++)
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] = *(ptr++);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//fourth ETC1-Block
						//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 4; x < 8; x++)
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] = *(ptr++);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";
				}
			}

			fclose(f);

			char out[1024];
			sprintf(out, "%s", argv[3]);
			FILE* fout = fopen(out, "wb");
			fwrite(buffer, 4, w*h, fout);
			fclose(fout);

			free(buffer);
		}
		else {
			u64 block;
			u32 pixels[4 * 4];
			int x, y, i, j, n, count;
			float percent;
			u64 alpha;
			u32* ptr;
			u32* buffer = (u32*)malloc(sizeof(u32)*w*h * 4);
			count = 0;

			n = (w / 4) * (h / 4);
			cout << fixed << setprecision(2) << "Decompressing... 0/" << n << " blocks | 0.00%";

			for (j = 0; j < h / 8; j++) {
				for (i = 0; i < w / 8; i++) {
					//first ETC1-Block
						//Alpha-values
					fread(&alpha, 8, 1, f);
					for (x = 0; x < 4; x++) {
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (y * w + x)] = ((((alpha & 0xF) << 28) | ((alpha & 0xF) << 24)) & 0xFF000000);
							alpha >>= 4;
						}
					}
					//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 0; x < 4; x++)
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] |= (*(ptr++) & 0xFFFFFF);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//second ETC1-Block
						//Alpha-values
					fread(&alpha, 8, 1, f);
					for (x = 4; x < 8; x++) {
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (y * w + x)] = ((((alpha & 0xF) << 28) | ((alpha & 0xF) << 24)) & 0xFF000000);
							alpha >>= 4;
						}
					}
					//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 0; x < 4; x++)
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] |= (*(ptr++) & 0xFFFFFF);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//third ETC1-Block
						//Alpha-values
					fread(&alpha, 8, 1, f);
					for (x = 0; x < 4; x++) {
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (y * w + x)] = ((((alpha & 0xF) << 28) | ((alpha & 0xF) << 24)) & 0xFF000000);
							alpha >>= 4;
						}
					}
					//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 4; x < 8; x++)
						for (y = 0; y < 4; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] |= (*(ptr++) & 0xFFFFFF);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//fourth ETC1-Block
						//Alpha-values
					fread(&alpha, 8, 1, f);
					for (x = 4; x < 8; x++) {
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (y * w + x)] = ((((alpha & 0xF) << 28) | ((alpha & 0xF) << 24)) & 0xFF000000);
							alpha >>= 4;
						}
					}
					//Pixels-values
					fread(&block, 8, 1, f);
					block = switchEndianness(block);
					rg_etc1::unpack_etc1_block(&block, pixels, false);
					ptr = pixels;
					for (x = 4; x < 8; x++)
						for (y = 4; y < 8; y++) {
							buffer[(j * 8 * w) + (i * 8) + (x * w + y)] |= (*(ptr++) & 0xFFFFFF);
						}

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rDecompressing... " << count << "/" << n << " blocks | " << percent << "%";
				}
			}

			fclose(f);

			char out[1024];
			sprintf(out, "%s", argv[3]);
			FILE* fout = fopen(out, "wb");
			fwrite(buffer, 4, w*h, fout);
			fclose(fout);

			free(buffer);
		}
	}
	else {
		rg_etc1::pack_etc1_block_init();
		if (alpha==0) {
			u32 pixels[4 * 4];
			u32 block;
			u32* ptr;
			u64* block2 = (u64*)malloc(sizeof(u64));
			int i, j, n, x, y, count;
			float percent;
			count = 0;

			char out[1024];
			sprintf(out, "%s", argv[3]);
			FILE* fout = fopen(out, "wb");

			n = (w / 4) * (h / 4);
			cout << fixed << setprecision(2) << "Compressing... 0/" << n << " blocks | 0.00%";

			for (y = 0; y < h / 8; y++) {
				for (x = 0; x < w / 8; x++) {
					//first 4*4 Pixelblock
						//Pixels values
					for (j = 0; j < 4; j++) {
						for (i = 0; i < 4; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[j * 4 + i] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//second 4*4 Pixelblock
						//Pixels values
					for (j = 0; j < 4; j++) {
						for (i = 4; i < 8; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[j * 4 + (i - 4)] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//third 4*4 Pixelblock
						//Pixels values
					for (j = 4; j < 8; j++) {
						for (i = 0; i < 4; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[(j - 4) * 4 + i] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//fourth 4*4 Pixelblock
						//Pixels values
					for (j = 4; j < 8; j++) {
						for (i = 4; i < 8; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[(j - 4) * 4 + (i - 4)] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";
				}
			}

			fclose(f);
			fclose(fout);
		}
		else {
			u8* alpha = (u8*)malloc(sizeof(u8));
			u8* pack = (u8*)malloc(sizeof(u8));
			u32* ptr;
			u64* block2 = (u64*)malloc(sizeof(u64));
			u32 pixels[4 * 4];
			u32 block;
			int i, j, x, y, n, count;
			float percent;
			count = 0;

			char out[1024];
			sprintf(out, "%s", argv[3]);
			FILE* fout = fopen(out, "wb");

			n = (w / 4) * (h / 4);
			cout << fixed << setprecision(2) << "Compressing... 0/" << n << " blocks | 0.00%";

			for (y = 0; y < h / 8; y++) {
				for (x = 0; x < w / 8; x++) {
					//first 4*4 Pixelblock
						//Alpha values
					for (j = 0; j < 4; j++) {
						for (i = 0; i < 4; i++) {
							if (i == 0 || i == 2) {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] = (alpha[0] & 0xF0) >> 4;
							}
							else {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] |= alpha[0] & 0xF0;
								fwrite(pack, 1, 1, fout);
							}
						}
					}

					//Pixels values
					for (j = 0; j < 4; j++) {
						for (i = 0; i < 4; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[j * 4 + i] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//second 4*4 Pixelblock
						//Alpha values
					for (j = 4; j < 8; j++) {
						for (i = 0; i < 4; i++) {
							if (i == 0 || i == 2) {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] = (alpha[0] & 0xF0) >> 4;
							}
							else {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] |= alpha[0] & 0xF0;
								fwrite(pack, 1, 1, fout);
							}
						}
					}

					//Pixels values
					for (j = 0; j < 4; j++) {
						for (i = 4; i < 8; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[j * 4 + (i - 4)] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//third 4*4 Pixelblock
						//Alpha values
					for (j = 0; j < 4; j++) {
						for (i = 4; i < 8; i++) {
							if (i == 4 || i == 6) {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] = (alpha[0] & 0xF0) >> 4;
							}
							else {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] |= alpha[0] & 0xF0;
								fwrite(pack, 1, 1, fout);
							}
						}
					}

					//Pixels values
					for (j = 4; j < 8; j++) {
						for (i = 0; i < 4; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[(j - 4) * 4 + i] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";

					//fourth 4*4 Pixelblock
						//Alpha values
					for (j = 4; j < 8; j++) {
						for (i = 4; i < 8; i++) {
							if (i == 4 || i == 6) {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] = (alpha[0] & 0xF0) >> 4;
							}
							else {
								fseek(f, y * 8 * 4 * w + x * 8 * 4 + i * (4 * w) + j * 4 + 3, 0);
								fread(alpha, 1, 1, f);
								pack[0] |= alpha[0] & 0xF0;
								fwrite(pack, 1, 1, fout);
							}
						}
					}

					//Pixels values
					for (j = 4; j < 8; j++) {
						for (i = 4; i < 8; i++) {
							fseek(f, y * 8 * 4 * w + x * 8 * 4 + j * (4 * w) + i * 4, 0);
							block = 0x00000000;
							fread(&block, 3, 1, f);
							block = block + 0xFF000000;
							pixels[(j - 4) * 4 + (i - 4)] = block;
						}
					}
					ptr = pixels;
					rg_etc1::pack_etc1_block(block2, ptr, rg_etc1::etc1_pack_params::etc1_pack_params());
					block2[0] = switchEndianness(block2[0]);
					fwrite(block2, 8, 1, fout);

					count++;
					n = (w / 4) * (h / 4);
					percent = (float)count * 100 / (float)n;
					cout << "\rCompressing... " << count << "/" << n << " blocks | " << percent << "%";
				}
			}
			fclose(f);
			fclose(fout);
		}
	}

	return 0;
}