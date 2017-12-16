#include <string.h>

#include "dict.h"
#include "sprite.h"
#include "error_handling.h"

struct _Dict {
  void **keys;
  void **values;
  int max_size;
  int current_size;
  key_cmp_func kcf;
  value_cmp_func vcf;
  value_free_func vff;
};

/* Private functions */
Dict *_dict_allocate_mem(int size);
int _dict_key_index(Dict *d, void *key);

/* Public funtions */
Dict* dict_new(int size, key_cmp_func kcf, value_cmp_func vcf, value_free_func vff){
  if(!kcf || !vcf || !vff){
    HE("invalid parameters", "dict_new")
    return NULL;
  }
  Dict *d = _dict_allocate_mem(size);
  d->max_size = size;
  d->kcf = kcf;
  d->vcf = vcf;
  d->vff = vff;
  d->current_size = 0;
  return d;
}

void *dict_get(Dict *d, void *key){
  if(!d || !key){
    HE("invalid parameters, key or dict is NULL", "dict_get")
    return NULL;
  }
  int index = _dict_key_index(d, key);
  if (index != UINT_ERROR){
    return d->values[index];
  }
  return NULL;
}

int dict_set(Dict *d, void *key, void *value){
  if (!d || !key || !value){
    HE("invalid parameters", "dict_set")
    return UINT_ERROR;
  }
  int index;
  if ((index = _dict_key_index(d, key)) != UINT_ERROR){
    d->values[index] = value;
  }
  if(d->current_size >= d->max_size){
    HE("no more elements fit into the dictionary", "dict_set")
    return UINT_ERROR;
  }
  index = d->current_size;
  d->keys[index] = key;
  d->values[index] = value;
  d->current_size++;
  return d->current_size;
}

void dict_destroy(Dict *d){
  if(!d) return;
  free(d->values);
  free(d->keys);
  free(d);
}

int string_cmp(void *p1, void *p2){
  return strcmp((char *) p1, (char *) p2);
}

int sprite_cmp(void *s1, void *s2){
  return s1-s2;
}

void free_sprite(void *spr){
  sprite_destroy((Sprite *) spr);
}
/*------------------------------*/

int _dict_key_index(Dict *d, void *key){
  for (int i = 0; i < d->current_size; ++i){
    if(d->kcf(d->keys[i], key) == 0)
      return i;
  }
  return UINT_ERROR;
}

Dict *_dict_allocate_mem(int size){
  if (!size) return NULL;
  /*Dict *d = oopsalloc(1, sizeof(Dict), "_dict_allocate_mem");
  d->keys = oopsalloc(size, sizeof(void *),"_dict_allocate_mem");
  d->values = oopsalloc(size, sizeof(void *),"_dict_allocate_mem");
  return d;*/return NULL;
}
