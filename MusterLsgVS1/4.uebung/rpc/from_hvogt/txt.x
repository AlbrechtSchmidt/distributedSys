const MAX_STR = 256;

struct text_record {
  string element_text<MAX_STR>;
};

program TXTPROG {
  version TXTVERS {
    int AddElement(string) = 1;
    string ShowElement(int) = 2;
  } = 1;
} = 0x20000001;

