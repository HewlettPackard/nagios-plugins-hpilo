#include<string.h>

#ifdef __cplusplus
extern "C" {
#endif
  int base64_encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize);
  int base64_decode(char *in, size_t inLen, unsigned char *out, size_t *outLen);

#ifdef __cplusplus
}
#endif
