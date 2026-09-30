/* us_xfr.h

   Header file for us_xfr_sv.c and us_xfr_cl.c.

   These programs employ a socket in /tmp. This makes it easy to compile
   and run the programs. However, for a security reasons, a real-world
   application should never create sensitive files in /tmp. (As a simple of
   example of the kind of security problems that can result, a malicious
   user could create a file using the name defined in SV_SOCK_PATH, and
   thereby cause a denial of service attack against this application.
   See Section 38.7 of "The Linux Programming Interface" for more details
   on this subject.)
*/
#include "tlpi_hdr.h"
#include <sys/socket.h>
#include <sys/un.h>

#define SV_SOCK_PATH "/tmp/us_xfr"

#define BUF_SIZE 100
