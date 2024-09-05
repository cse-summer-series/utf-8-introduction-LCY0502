#include <stdio.h>
#include <string.h>

unsigned int num_bytes(char b) {
  //    0xxxxxxx        1xxxxxxx
  //  & 10000000        10000000
  //  ----------        --------
  //    00000000        10000000
  if((b & 0b10000000) == 0) {
    return 1;
  }
  // & is a "bitwise operator"
  // This overall strategy is called "bitmasking"
  //   110xxxxx         010xxxxx    100xxxxx    111xxxxx ...
  // & 11100000
  // ----------         --------    --------    --------
  //   11000000         01000000    10000000    11100000
  else if((b & 0b11100000) == 0b11000000) { return 2; }
  else if((b & 0b11110000) == 0b11100000) { return 3; }
  else if((b & 0b11111000) == 0b11110000) { return 4; }
  else if((b & 0b11111100) == 0b11111000) { return 5; }
  else if((b & 0b11111110) == 0b11111100) { return 6; }
  else { return -1; }
}

unsigned int utf8_strlen(char* unicode) {
  unsigned int len = strlen(unicode); // count of bytes in this string
  unsigned int bytes_seen = 0;
  unsigned int unicode_characters_seen = 0;
  while(bytes_seen < len) {
    unsigned int bytes_this_char = num_bytes(unicode[bytes_seen]);
    bytes_seen += bytes_this_char;
    unicode_characters_seen += 1;
  }
  return unicode_characters_seen;
}

/**
 * Returns the number of bytes taken by the first n UTF-8 characters
 * in the given UTF-8 string
 * 
 * This is useful for later work with UTF-8 strings where we may need
 * to allocate or copy substrings based on UTF-8 character indices!
 * 
 * If the value of n is greater than the number of unicode characters,
 * returns -1.
 * 
 * Examples:
 *   bytes_for("José", 3) -> 3
 *   bytes_for("Ülo", 3) -> 4
 *   bytes_for("José", 4) -> 5
 *   bytes_for("成龙", 1) -> 3
 *   bytes_for("成龙", 2) -> 6
 *   bytes_for("成龙", 3) -> -1
 */
unsigned int bytes_for(char* unicode, unsigned int n) {
    unsigned int ulen = utf8_strlen(unicode); //count total # of utf8 characters
    unsigned int seen_bytes = 0;

    if(ulen < n) {
        return -1;
    }

    for(int i = 0; i < n; i++)
    {
        seen_bytes += num_bytes(unicode[seen_bytes]);
    }
    
    return seen_bytes;
}

int main(int argc, char** argv) {
    printf("Instruction | expected result | test\n");
    printf("bytes_for(\"José\", 3) | 3 | %d\n", bytes_for("José", 3));
    printf("bytes_for(\"Ülo\", 3) | 4 | %d\n",  bytes_for("Ülo", 3));
    printf("bytes_for(\"José\", 4) | 5 | %d\n", bytes_for("José", 4));
    printf("bytes_for(\"成龙\", 1) | 3 | %d\n", bytes_for("成龙", 1));
    printf("bytes_for(\"成龙\", 2) | 6 | %d\n", bytes_for("成龙", 2));
    printf("bytes_for(\"成龙\", 3) | -1 | %d\n",bytes_for("成龙", 3));
    return 0;
}



/*
  Some extra code for reference that's not part of the starter above:


  unsigned char li[4] = "李";
  printf("%d(%x) should be 1 byte long: %d\n", 'a', 'a', num_bytes('a'));
  printf("%d(%x) should be 3 bytes long: %d\n", li[0], li[0], num_bytes(li[0]));
  unsigned char e[3] = "é";
  printf("%d(%x) should be 2 bytes long: %d\n", e[0], e[0], num_bytes(e[0]));



*/

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters José
// Hi José, your name is 5 characters long according to strlen.
// The first character by indexing is J
// 5 characters long? That seems weird
/*
    74(4a) 111(6f) 115(73) 195(c3) 169(a9)

    4a -> (0100) (1010) // start with 0!
    6f -> (0110) (1111) // start with 0!
    73 -> (0111) (0011) // start with 0!
    c3 -> (1100) (0011) // start with 1!
    a9 -> (1010) (1001) // start with 1!
*/

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters Ülo
// Hi Ülo, your name is 4 characters long according to strlen.
// The first character by indexing is �
// What is this question mark character?

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters 李
// Hi 李, your name is 3 characters long according to strlen.
// The first character by indexing is �
// Too long and question mark
/*
    230(e6) 157(9d) 142(8e)

    e6 -> (1110) (0110)
    9d -> (1001) (1100)
    8e -> (1000) (1110)
*/