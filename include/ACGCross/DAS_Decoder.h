
/* Create index of dictionary.
*param:A pointer point to dictionary.
*param:A pointer point to a 256 bytes space to output dictionary index.
*/
void DAS_CreateDictionaryIndex(const void* dic,void* dic_index);

/* Decode encoded data.
*param:A pointer point to dictionary index,it should be created by DAS_CreateDictionaryIndex().
*param:A pointer point to dictionary.
*param:A pointer point to encoded data.
*param:A pointer point to a space to output original data,the size of space is same to encoded data.
*param:Data size.
*/
void DAS_Decode(const void* dic_index,const void* dic,const void* encoded_data,void* original_data,int size);
