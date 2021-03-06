#include "rastmap.h"
#include "queue.h"
#include "move.h"

#define DISTDOMAIN 100000.0f
#define DISTROAD 0.0f
#define DIRDOMAIN 0
#define DIRROAD 5
#define DROPDOMAIN 0
#define DROPROAD 0


extern queue **prepare_input ( cell_map *,     /* road   */
                           cell_map *,     /* domain */
                           cell_map *,     /* dist   */
                           cell_map *,     /* dir    */
                           cell_map *,     /* up     */
                           cell_map *,     /* dw     */
                           seg_map *);     /* segment map info */

extern int distdrop ( cell_map *,     /* elev   */
                      cell_map *,     /* dist   */
                      cell_map *,     /* dir    */
                      cell_map *,     /* up     */
                      cell_map *,     /* dw     */
                      seg_map *,      /* segment map info */
                      move *,         /* movements */
                      queue **);      /* redo_segments */

extern int print_dir ( cell_map *, seg_map * );

