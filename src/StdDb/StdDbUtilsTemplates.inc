template<typename T> void WriteBigEndian(T Data, unsigned char* DestionationStart){
  uint32_t index = 0, BitsShift = 0;
  for(index = 0; index<sizeof(Data); index++){
    BitsShift = (sizeof(DestionationStart) - sizeof(char)*(index+1)) * 8;
    DestionationStart[index] = (Data >> BitsShift);
  }
}

template<typename T> void WriteLittleEndian(T Data, unsigned char* DestionationStart){
  uint32_t index = 0, BitsShift = 0;
  for(index = 0; index<sizeof(Data); index++){
    BitsShift = (sizeof(char)*index) * 8;
    *(DestionationStart+index) = (Data >> BitsShift);
  }
}