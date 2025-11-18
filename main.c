#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 100
#define DELIM ";"// CSV delimiter

char *ifile, *ofile; // pointers til dem der holder input fil og output fil
unsigned filter_age_max;
FILE *istream, *ostream; //peger på de åbne filer

const char  
  R"(Filters CSV rows, keeping only those with provided maximum age
%1$s max-age [input-file] [output-file]

Example: 
%1$s max-age input.csv output.csv
%1$s max-age input.csv (output to stdout)
%1$s max-age           (input from stdin, output to stdout)
)";

//tager de 2 input filer ind
void filter_stream(FILE *istream, FILE *ostream) {
  char line[LINE_MAX];
  char *fgets_return;
  char *name, *age_str;
  size_t line_no = 0;

  //læser linje fra istream ind i lina array
  while ((fgets_return = fgets(line, LINE_MAX, istream)) != NULL)
      // Read a line from `istream` and assign the return value to
      // `fgets_return`
  {
    ++line_no;
//tjekker den ikke er null og at første linje ik en newline symbol
    if (fgets_return && *fgets_return != '\n') {
        if (strlen(line) > 1) {
            // Assign `name` and `age_str` using `strtok`
            // strtok deler linjen ved første ';' og returnerer navnet
            name = strtok(line, DELIM);
            //returner næste token på linjen så alderen
            age_str = strtok(NULL, DELIM);

    //hvis alder er tom
            if (!age_str) {
            // Error message
            fprintf(stderr, "Line %zu: missing age field.\n", line_no);
            continue;

            }
        }
    } else {
      // Error message
      // YOUR CODE HERE
        //så er der ikke noget på linjen  og der er fejl
      fprintf(stderr, "Line %zu: invalid age value '%s'.\n", line_no, age_str);
      continue;
    }

    // Age processing
    unsigned age;
    int recognized_count = sscanf(age_str, "%d", &age);
    if (recognized_count == 1) {
      if (age <= filter_age_max) {
        // Forward input line to `ostream`
        // YOUR CODE HERE
        //printer navn og alder ind i output filen
        fprintf(ostream, "%s,%u\n", name, age);
      }
    } else {
      // Error message
      // YOUR CODE HERE
      //ellers printer jeg i terminalen hvilken linje der mangler noget
      fprintf(stderr, "Line %zu: invalid age value '%s'.\n", line_no, age_str);
    }

  }
}


int main(int argc, char *argv[]) {
  switch (argc) {
    //kører casesne så hvis der er mindst den mængde argumenter
  case 4:
    // max-age ifile ofile
    //har 4 argumenter så output er det 4
    ofile = argv[3];
  case 3:
    // max-age ifile
    ifile = argv[2];
  case 2:
    // max-age
    if (!sscanf(argv[1], "%d", &filter_age_max)) {
      puts("First argument is not an age.");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    printf(USAGE, argv[0]);
    return EXIT_SUCCESS;
  }

  //hvis jeg har inputfil
  if (ifile) {
    // Open `ifile` and assign it to `istream`
    // YOUR CODE HERE
    //åben filen i read-mode
    istream = fopen(ifile, "r");

    // Exit program with an error message if file cannot be opened
    // YOUR CODE HERE
    //ellers fejlkode hvis der ikke findes den fil
        if (!istream) {
      perror("Error opening input file");
      exit(EXIT_FAILURE);
    }
  } else {
    // Set `ostream` if no file provided
    // YOUR CODE HERE
    //ellers sættes istream til standardinputtet
    istream = stdin;
  }

  //Hvis der er outputfil
  if (ofile) {
    // Open `ofile` and assign it to `ostream`
    // YOUR CODE HERE
    //åben i skrivning mode
        ostream = fopen(ofile, "w");

    // Exit program with an error message if file cannot be opened
    // YOUR CODE HERE
        if (!ostream) {
      perror("Error opening output file");
      exit(EXIT_FAILURE);
    }
  } else {
    // Set `ostream` if no file provided
    // YOUR CODE HERE
    //hvis der ikke er outputfil sættes den til standardoutput i terminal
    ostream = stdout;
  }

  //så kører jeg funktion med program
  filter_stream(istream, ostream);
}

