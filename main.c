#include "list.h"

int
main()
{
  list *l = create_empty_list();
  populate(10, l);
  print_list(l);
  free_list(l);
  return 0;
}

