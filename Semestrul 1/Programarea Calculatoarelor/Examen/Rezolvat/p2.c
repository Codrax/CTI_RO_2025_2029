#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint16_t an;
	uint8_t luna, zi;
} data;
typedef struct {
	data data;
	float curs;
} curs_zi;

#define BUF_DEFAULT 128

int8_t sign(int nr) {
	return nr / abs(nr);
}

// inutil
int8_t compare_date(data* a, data* b) {
	if (a->an == b->an) {
		if (a->luna == b->luna) {
			if (a->zi == b->zi) {
				return 0; //equal
			}
			else return sign(a->zi - b->zi);	
		}
		else return sign(a->luna - b->luna);
	}
	else return sign(a->an - b->an);
}

void BubbleSort(curs_zi *v, int n) {
	int s,i;
	curs_zi aux;
	do{
		s = 0;                             // setam faptul ca nu au avut loc schimbari la iterarea curenta
		for (i = 1; i < n; i++)
		{      
			// @1 - pornim de la 1 ca sa formam perechi (v[i-1],v[i])
			if (v[i - 1].curs > v[i].curs)
			{             
				// daca valorile NU respecta relatia de ordine
				aux = v[i - 1];                 // interschimba valorile
				v[i - 1] = v[i];
				v[i] = aux;
				s = 1;                             // @2 - seteaza faptul ca a avut loc cel putin o schimbare
			}
		}
	} while (s);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Parametrul(1) al fisierului citit este necesar.\n"); exit(1);
	}
	if (argc < 3) {
		fprintf(stderr, "Parametrul(2) al fisierului scris este necesar.\n"); exit(1);
	}
	FILE *in = fopen(argv[1], "r");
	if (in == NULL) {
		perror("Eroare interna"); exit(2);
	}

	// Citire fisier
	curs_zi p, *list=malloc(BUF_DEFAULT * sizeof(curs_zi));
	size_t listSize=BUF_DEFAULT, total = 0;
	while (fscanf(in, "%hhu.%hhu.%hu;%e", &p.data.zi, &p.data.luna, &p.data.an, &p.curs) == 4) {
		// Re-alloc
		if (total >= listSize) {
			listSize *= 2;
			curs_zi *old=list;
			list = realloc(list, listSize * sizeof(curs_zi));
			if (list == NULL) {
				free(old); // free old memory
				perror("Eroare alocare memorie");
				fclose(in);
				exit(4);
			}
		}

		// Write value
		list[total].data.an = p.data.an;
		list[total].data.luna = p.data.luna;
		list[total].data.zi = p.data.zi;
		list[total].curs = p.curs;

		// Next
		total++;
	}

	// Closedata
	fclose(in);

	// Sortare
	BubbleSort(list, total);

	// Scriere in fisierdata
	FILE* out = fopen(argv[2], "w");
	if (out == NULL) {
		free(list);
		perror("Eroare interna"); exit(2);
	}

	for (size_t i=0; i<total; i++) {
		fprintf(out, "%hhu.%hhu.%hu;%e\n", 
			list[i].data.zi, list[i].data.luna, list[i].data.an, list[i].curs);
	}

	// Close
	fclose(out);

	// Free
	free(list);

	// Done
	//printf("\nDone!\n");
}
