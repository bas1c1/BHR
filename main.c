#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define ll long long int

typedef union {
  uint8_t b[4];
  uint32_t i;
} i32;

ll to_int(char *s) {
  ll res = 0;
  ll sign = *s == '-' ? (++s, -1) : (s += *s == '+', 1);
  if (*s >= '1' && *s <= '9')
    for (--s; *++s && *s >= '0' && *s <= '9' && !(res >> 32);) 
      res = res * 10 + (*s - '0');
  return *s || res >> 32 ? 0 : res * sign;
}

void btos(char *buf, i32 bytes) {
  for (int i = 0; i < 4; i++) {
    if (bytes.b[i] <= 13 || bytes.b[i] >= 255) {
      buf[i] = '.';
    } else {
      buf[i] = (char)bytes.b[i];
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "rb");

  fseek(f, 0, SEEK_END);
  ll sz = (ll)ftell(f);
  fseek(f, 0, 0);
  
  if (argc > 2)
    sz = to_int(argv[2]);

  i32 counter;
  counter.i = 0;
  
  for (ll i = 0; i < (ll)(sz/16); i++) {
    i32 byte_str[4];

    fread(&byte_str[0].b, sizeof(i32), 1, f);
    fread(&byte_str[1].b, sizeof(i32), 1, f);
    fread(&byte_str[2].b, sizeof(i32), 1, f);
    fread(&byte_str[3].b, sizeof(i32), 1, f);

    char buf[17];
    buf[16] = '\0';

    printf("%02X%02X%02X%02X | ", counter.b[3], counter.b[2], counter.b[1], counter.b[0]);
    
    for (int j = 0; j < 4; j++ ) {
      btos(buf+(j*4), byte_str[j]);
      printf("%02X %02X %02X %02X  ", byte_str[j].b[0], byte_str[j].b[1], byte_str[j].b[2], byte_str[j].b[3]);
    }

    printf("   | %s\n", buf);
    free(buf);

    counter.i += 16;
  }

  fclose(f);
  return 0;
}
