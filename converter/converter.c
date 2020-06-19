#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// checks if input is valid
bool valid(const char *temperature) {
 bool result = true;
 for (int i=0; i<strlen(temperature); i++) {
 if (! isdigit(temperature[i])) result = false;
}
 int n = atoi(temperature);
 if (n > 0 && temperature[0] == '0') result = false;
 else if (n < 0 && temperature[0] == '-' && temperature[1] == '0') result = false;
 else if (n < 0) result = true;
 return result;
}


// function to allow combination  of two char's to be returned
char* concat(char *a, char *b) {
 char *result = malloc(strlen(a)+strlen(b)+1);
 strcpy(result, a);
 strcat(result, b);
 return result;
 }

// Converts celsius to fahrenheit or vice versa
char *converter(const char *temperature,  const char *initialtype) {
 long int n;
n = atoi(temperature);
char *answer = "invalid type";
if (! valid(temperature)) answer = "invalid input";

else {
// converts celsius to fahrenheit
char *celsius = "celsius";
int test0;
test0 = strcmp(initialtype, celsius);
if (test0 == 0) {
    char buffer[80];
    double temp = ((n * 1.8) + 32) ;
    sprintf(buffer,"%0.1f",temp);
    answer = concat( buffer, "°F");
  }

// converts fahrenheit to celsius
char *fahrenheit = "fahrenheit";
int test1;
test1 = strcmp(initialtype, fahrenheit);
if (test1 == 0) {
    char buffer1[80];
    double temp = ( n -32) * (5.0 / 9.0) ;
    sprintf(buffer1,"%0.1f",temp);
    answer = concat( buffer1, "°C");
  }
}


    return answer;


}


// Increment the test number and check that two strings are equal.
int eq(int n, const char *actual, const char *expected) {
    n++;
    if (strcmp(actual, expected) != 0) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %s\n", actual);
        fprintf(stderr, "instead of: %s\n", expected);
        exit(1);
    }
    return n;
}

// Run tests on the converter function.
void test() {
    int n = 0;

    // Tests 1 to .. : checks °C to °F conversion
    n = eq(n, converter("0", "celsius"), "32.0°F");
    n = eq(n, converter("10", "celsius"), "50.0°F");
    n = eq(n, converter("151", "celsius"), "303.8°F");
    n = eq(n, converter("-1", "celsius"), "30.2°F");
    n = eq(n, converter("-200", "celsius"), "-328.0°F");

    // Tests .. to .. : checks °F to °C conversion
    n = eq(n, converter("0", "fahrenheit"), "-17.8°C");
    n = eq(n, converter("10", "fahrenheit"), "-12.2°C");
    n = eq(n, converter("110", "fahrenheit"), "43.3°C");
    n = eq(n, converter("-15", "fahrenheit"), "-26.1°C");
    n = eq(n, converter("-300", "fahrenheit"), "-184.4°C");

    // Checks if input is valid
    n = eq(n, converter("a", "celsius"), "invalid input");
    n = eq(n, converter("a", "fahrenheit"), "invalid input");
    n = eq(n, converter("70a", "celsius"), "invalid input");
    n = eq(n, converter("70a", "fahrenheit"), "invalid input");
    n = eq(n, converter("01", "celsius"), "invalid input");
    n = eq(n, converter("01", "fahrenheit"), "invalid input");
    n = eq(n, converter("-01", "celsius"), "invalid input");
    n = eq(n, converter("-01", "fahrenheit"), "invalid input");

    // checks if type (e.g. celsius or fahrenheit) is valid
    n = eq(n, converter("0", "kelvin"), "invalid type");
    n = eq(n, converter("0", "meters"), "invalid type");


    // check the upper limits of temperature
    n = eq(n, converter("2147483647", "celsius"), "3865470596.6°F");
    n = eq(n, converter("2147483647", "fahrenheit"), "1193046452.8°C");
    n = eq(n, converter("-2147483647", "celsius"), "-3865470532.6°F");
    n = eq(n, converter("-2147483647", "fahrenheit"), "-1193046488.3°C");

    printf("Tests passed: %d\n", n);
}



int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    if (n == 1) test();
    else if (n == 3) printf("%s\n", converter(args[1], args[2]));
    else {
        fprintf(stderr, "Use: e.g  ./converter 30 celsius \n");
	fprintf(stderr, "or ./converter 70 fahrenheit \n");
        exit(1);
    }
    return 0;
}
