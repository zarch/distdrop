#include <stdio.h>
#include "list.h"
#include "test.h"

int
main()
{
  list *l = create_empty_list();
  populate_inverse_rows_cols(10, 3, l);
  print_list(l);
  free_list(l);

  /* test the function to add element in a order list */
  printf("\nTest the function to add element in a order list\n\n");
  list *o = create_empty_list();
  order_populate_inverse_rows_cols(10, 3, o);
  print_list(o);
  free_list(o);
  return 0;
}

