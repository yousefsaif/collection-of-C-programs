#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"

struct state{
 int x;
 int y;
 int x0;
 int y0;
 bool pen;
};

typedef struct state State;

//updates the x-axis values
void DX(State *S, int dx){
    S->x0 = S->x;
    S->x = S->x + dx;
}

//updates y-axis values and draws lines (also handles colour)
void DY(State *S, int dy, display *d, uint32_t opcodeP, int32_t operandP){
  if(operandP == -1){
    S->y0 = S->y;
    S->y = S->y + dy;
    if(S->pen == true){
      line(d, S->x0, S->y0, S->x, S->y);
      S->x0 = S->x;
      S->y0 = S->y;
    }
  }
  else if(opcodeP == 6){
    S->y0 = S->y;
    S->y = S->y + dy;
    if(S->pen == true){
      cline(d, S->x0, S->y0, S->x, S->y, operandP);
      S->x0 = S->x;
      S->y0 = S->y;
    }
  }
}

// handles any pauses when sketching
void DT(State *S, int dt, display *d){
  pause(d, dt);
  S->x0 = S->x;
  S->y0 = S->y;
  }

//Change Pen from up to down or vice-versa
void PEN(State *S){
  S->pen = ! S->pen;
  S->x0 = S->x;
  S->y0 = S->y;
}

//function to extract opcode
uint32_t extractopcode(uint32_t a) {
  a = (a >> 6) &0x3;
  return a;
}

//function to extract operand
int32_t extractoperand(uint32_t a) {
  a = a&0x3F;
  int32_t b = a;
  if ((b >> 5) & 1  ){
    int32_t neg = -1;
    b = (neg << 6)| b;
  }
  return b;
}

//function to extract operand for DT function
int32_t extractoperandDT(uint32_t a) {
  a = a&0x3F;
  return a;
}

// function to extract length
int32_t extractlength(int32_t a) {
  int b  =  (a >> 4)&0x3;
  if(b == 3){
    b = 4;
  }
  return b;
}

// function to extract opcode (extended format)
int32_t extractopcodeextended(uint32_t a) {
  a = a&0xF;
  return a;
}

// function to extract operand (extended format)
int32_t extractoperandextended(int32_t n, FILE *in) {
  uint32_t a;
  int32_t result = 0;
  for (int i = 0; i<n; i++){
    a = fgetc(in);
    if(i == 0 && ( a & 0x80) != 0){
      result = -1;
    }
    result = (result << 8) | a;
  }
  return result;
}

// function to interpret file instructions
void interpret(FILE *in, display *d) {
  uint32_t a, opcode, opcodeP;
  int32_t operand, operandDT;
  int32_t operandP = -1;
  State rs = {0, 0, 0, 0, false};
  State *S = &rs;
  for (int32_t c = fgetc(in); c != EOF; c = fgetc(in)){
      a = c;
      opcode = extractopcode(a);
      operand = extractoperand(a);
      operandDT = extractoperandDT(a);
      if(opcode == 0){
        DX(S, operand);
      }
      else if(opcode == 1){
        DY(S,operand,d,opcodeP,operandP);
      }
      else if(opcode == 2){
        DT(S, operandDT, d);
      }
      else{
            uint32_t n;
            n = extractlength(a);
            opcode = extractopcodeextended(a);
            operand = extractoperandextended(n, in);
            if(opcode == 0){
              DX(S, operand);
            }
            else if(opcode == 1){
              DY(S,operand,d,opcodeP, operandP);
            }
            else if(opcode == 2){
              DT(S, operand, d);
            }
            else if(opcode == 3){
              PEN(S);
            }
            else if (opcode == 4) {
              clear(d);
            }
            else if (opcode == 5) {
              key(d);
            }
            else if (opcode == 6){
              operandP = operand;
              opcodeP = 6;
            }
      }
  }
}

// Read sketch instructions from the given file.  If test is NULL, display the
// result in a graphics window, else check the graphics calls made.
void run(char *filename, char *test[]) {
    FILE *in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(1);
    }
    display *d = newDisplay(filename, 200, 200, test);
    interpret(in, d);
    end(d);
    fclose(in);
}

// ----------------------------------------------------------------------------
// Nothing below this point needs to be changed.
// ----------------------------------------------------------------------------

// Forward declaration of test data.
char **lineTest, **squareTest, **boxTest, **oxoTest, **diagTest, **crossTest,
     **clearTest, **keyTest, **pausesTest, **fieldTest, **lawnTest;

void testSketches() {
    // Stage 1
    run("line.sketch", lineTest);
    run("square.sketch", squareTest);
    run("box.sketch", boxTest);
    run("oxo.sketch", oxoTest);

    // Stage 2
    run("diag.sketch", diagTest);
    run("cross.sketch", crossTest);

    // Stage 3
    run("clear.sketch", clearTest);
    run("key.sketch", keyTest);

    // Stage 4
    run("pauses.sketch", pausesTest);
    run("field.sketch", fieldTest);
    run("lawn.sketch", lawnTest);
}

int main(int n, char *args[n]) {
    if (n == 1) testSketches();
    else if (n == 2) run(args[1], NULL);
    else {
        fprintf(stderr, "Usage: sketch [file.sketch]");
        exit(1);
    }
}

// Each test is a series of calls, stored in a variable-length array of strings,
// with a NULL terminator.

// The calls that should be made for line.sketch.
char **lineTest = (char *[]) {
    "line(d,30,30,60,30)", NULL
};

// The calls that should be made for square.sketch.
char **squareTest = (char *[]) {
    "line(d,30,30,60,30)", "line(d,60,30,60,60)",
    "line(d,60,60,30,60)","line(d,30,60,30,30)", NULL
};

// The calls that should be made for box.sketch.
char **boxTest = (char *[]) {
    "line(d,30,30,32,30)", "pause(d,10)", "line(d,32,30,34,30)", "pause(d,10)",
    "line(d,34,30,36,30)", "pause(d,10)", "line(d,36,30,38,30)", "pause(d,10)",
    "line(d,38,30,40,30)", "pause(d,10)", "line(d,40,30,42,30)", "pause(d,10)",
    "line(d,42,30,44,30)", "pause(d,10)", "line(d,44,30,46,30)", "pause(d,10)",
    "line(d,46,30,48,30)", "pause(d,10)", "line(d,48,30,50,30)", "pause(d,10)",
    "line(d,50,30,52,30)", "pause(d,10)", "line(d,52,30,54,30)", "pause(d,10)",
    "line(d,54,30,56,30)", "pause(d,10)", "line(d,56,30,58,30)", "pause(d,10)",
    "line(d,58,30,60,30)", "pause(d,10)", "line(d,60,30,60,32)", "pause(d,10)",
    "line(d,60,32,60,34)", "pause(d,10)", "line(d,60,34,60,36)", "pause(d,10)",
    "line(d,60,36,60,38)", "pause(d,10)", "line(d,60,38,60,40)", "pause(d,10)",
    "line(d,60,40,60,42)", "pause(d,10)", "line(d,60,42,60,44)", "pause(d,10)",
    "line(d,60,44,60,46)", "pause(d,10)", "line(d,60,46,60,48)", "pause(d,10)",
    "line(d,60,48,60,50)", "pause(d,10)", "line(d,60,50,60,52)", "pause(d,10)",
    "line(d,60,52,60,54)", "pause(d,10)", "line(d,60,54,60,56)", "pause(d,10)",
    "line(d,60,56,60,58)", "pause(d,10)", "line(d,60,58,60,60)", "pause(d,10)",
    "line(d,60,60,58,60)", "pause(d,10)", "line(d,58,60,56,60)", "pause(d,10)",
    "line(d,56,60,54,60)", "pause(d,10)", "line(d,54,60,52,60)", "pause(d,10)",
    "line(d,52,60,50,60)", "pause(d,10)", "line(d,50,60,48,60)", "pause(d,10)",
    "line(d,48,60,46,60)", "pause(d,10)", "line(d,46,60,44,60)", "pause(d,10)",
    "line(d,44,60,42,60)", "pause(d,10)", "line(d,42,60,40,60)", "pause(d,10)",
    "line(d,40,60,38,60)", "pause(d,10)", "line(d,38,60,36,60)", "pause(d,10)",
    "line(d,36,60,34,60)", "pause(d,10)", "line(d,34,60,32,60)", "pause(d,10)",
    "line(d,32,60,30,60)", "pause(d,10)", "line(d,30,60,30,58)", "pause(d,10)",
    "line(d,30,58,30,56)", "pause(d,10)", "line(d,30,56,30,54)", "pause(d,10)",
    "line(d,30,54,30,52)", "pause(d,10)", "line(d,30,52,30,50)", "pause(d,10)",
    "line(d,30,50,30,48)", "pause(d,10)", "line(d,30,48,30,46)", "pause(d,10)",
    "line(d,30,46,30,44)", "pause(d,10)", "line(d,30,44,30,42)", "pause(d,10)",
    "line(d,30,42,30,40)", "pause(d,10)", "line(d,30,40,30,38)", "pause(d,10)",
    "line(d,30,38,30,36)", "pause(d,10)", "line(d,30,36,30,34)", "pause(d,10)",
    "line(d,30,34,30,32)", "pause(d,10)", "line(d,30,32,30,30)", "pause(d,10)",
    NULL
};

// The calls that should be made for box.sketch.
char **oxoTest = (char *[]) {
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,40,60,40)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,50,60,50)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,40,30,40,60)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,50,30,50,60)", NULL
};

// The calls that should be made for diag.sketch.
char **diagTest = (char *[]) {
    "line(d,30,30,60,60)", NULL
};

// The calls that should be made for cross.sketch.
char **crossTest = (char *[]) {
    "line(d,30,30,60,60)", "line(d,60,30,30,60)", NULL
};

// The calls that should be made for clear.sketch.
char **clearTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for key.sketch.
char **keyTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "key(d)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for diag.sketch.
char **pausesTest = (char *[]) {
    "pause(d,0)", "pause(d,0)", "pause(d,127)", "pause(d,128)", "pause(d,300)",
    "pause(d,0)", "pause(d,71469)", NULL
};

// The calls that should be made for field.sketch.
char **fieldTest = (char *[]) {
    "line(d,30,30,170,30)", "line(d,170,30,170,170)",
    "line(d,170,170,30,170)", "line(d,30,170,30,30)", NULL
};

// The calls that should be made for field.sketch.
char **lawnTest = (char *[]) {
    "cline(d,30,30,170,30,16711935)", "cline(d,170,30,170,170,16711935)",
    "cline(d,170,170,30,170,16711935)", "cline(d,30,170,30,30,16711935)",
    NULL
};
