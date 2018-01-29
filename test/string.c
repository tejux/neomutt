#define TEST_NO_MAIN
#include "acutest.h"

#include "mutt/string2.h"

void test_string_strfcpy(void)
{
  char src[20] = "\0";
  char dst[10];

  { /* empty */
    size_t len = mutt_str_strfcpy(dst, src, sizeof(dst));
    if (!TEST_CHECK(len == 0))
    {
      TEST_MSG("Expected: %zu", 0);
      TEST_MSG("Actual  : %zu", len);
    }
  }

  { /* normal */
    const char trial[] = "Hello";
    mutt_str_strfcpy(src, trial, sizeof(src)); /* let's eat our own dogfood */
    size_t len = mutt_str_strfcpy(dst, src, sizeof(dst));
    if (!TEST_CHECK(len == sizeof(trial) - 1))
    {
      TEST_MSG("Expected: %zu", sizeof(trial) - 1);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(dst, trial) == 0))
    {
      TEST_MSG("Expected: %s", trial);
      TEST_MSG("Actual  : %s", dst);
    }
  }

  { /* too long */
    const char trial[] = "Hello Hello Hello";
    mutt_str_strfcpy(src, trial, sizeof(src));
    size_t len = mutt_str_strfcpy(dst, src, sizeof(dst));
    if (!TEST_CHECK(len == sizeof(dst) - 1))
    {
      TEST_MSG("Expected: %zu", sizeof(dst) - 1);
      TEST_MSG("Actual  : %zu", len);
    }
  }

  /* confirm that if we pass "dsize", we get a string len dsize-1 */
  {
    char bazz[] = "123456";
    size_t dsize = 4; // lets get a substring with a smaller size
    size_t len = mutt_str_strfcpy(dst, bazz, 4);
    if (!TEST_CHECK(strcmp(dst, "123") == 0))
    {
      TEST_MSG("Expected: %s", "123");
      TEST_MSG("Actual  : %s", dst);
    }
    if (!TEST_CHECK(len == (dsize - 1)))
    {
      TEST_MSG("Expected: %zu", (dsize - 1));
      TEST_MSG("Actual  : %zu", len);
    }
  }
}

void test_string_strnfcpy(void)
{
  const char src[] = "One Two Three Four Five";
  char dst[10];
  char big[32];

  { // bazzy. prove that if dsize > size, then dsize will be used, and
    // resulting string is (dsize - 1)
    char *src = "123456";
    char dest[4]; // "dsize" 4
    size_t len;
    len = mutt_str_strnfcpy(dest, src, sizeof(dest), 5);
    if (!TEST_CHECK(len == 3))
    {
      TEST_MSG("Expected: %zu", 3);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(dest, "123") == 0))
    {
      TEST_MSG("Expected: %s", "123");
      TEST_MSG("Actual  : %s", dest);
    }
  }

  { /* copy a substring */
    size_t len = mutt_str_strnfcpy(dst, src, 3, sizeof(dst));
    if (!TEST_CHECK(len == 2))
    {
      TEST_MSG("Expected: %zu", 2);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(dst, "On") == 0))
    {
      TEST_MSG("Expected: %s", "On");
      TEST_MSG("Actual  : %s", dst);
    }
  }

  { /* try to copy the whole available length */
    size_t len = mutt_str_strnfcpy(dst, src, sizeof(dst), sizeof(dst));
    if (!TEST_CHECK(len == sizeof(dst) - 1))
    {
      TEST_MSG("Expected: %zu", dst - 1);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(dst, "One Two T") == 0))
    {
      TEST_MSG("Expected: %s", "One Two T");
      TEST_MSG("Actual  : %s", dst);
    }
  }

  { /* try to copy more than it fits */
    size_t len = mutt_str_strnfcpy(dst, src, strlen(src), sizeof(dst));
    if (!TEST_CHECK(len == sizeof(dst) - 1))
    {
      TEST_MSG("Expected: %zu", dst - 1);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(dst, "One Two T") == 0))
    {
      TEST_MSG("Expected: %s", "One Two T");
      TEST_MSG("Actual  : %s", dst);
    }
  }

  { /* try to copy more than available */
    size_t len = mutt_str_strnfcpy(big, src, sizeof(big), sizeof(big));
    if (!TEST_CHECK(len == sizeof(src) - 1))
    {
      TEST_MSG("Expected: %zu", sizeof(src) - 1);
      TEST_MSG("Actual  : %zu", len);
    }
    if (!TEST_CHECK(strcmp(big, src) == 0))
    {
      TEST_MSG("Expected: %s", src);
      TEST_MSG("Actual  : %s", big);
    }
  }
}
