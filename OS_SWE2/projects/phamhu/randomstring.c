/*
 * Author: Huy Pham
 * Created date: 01/30/16
 * Program name: randomstring.c
 * Description: Both functions inputChar() and inputString() were updated in the testme.c program.
 * inputChar() was changed to generate a random character from space to ~ (ASCII value of 32 to 126).
 * inputString() was changed to shuffle the string 'reset' randomly. When the program testme.c is 
 * executed, it will check if the random character is meet with a specific list of characters(i.e. '['). 
 * If the character matches, the state of the program is increased. If the character doesn't match, the 
 * program will generate a new character and continue to check it against the list of characters. This 
 * will continue until the state of the program is 9. At that time, the random input string will be 
 * checked again 'reset' string. If the random string doesn't match, the program will continue to generate
 * the input string and check to see if it equals 'reset'. If the random string equals 'reset', the program 
 * will exit.
 */
