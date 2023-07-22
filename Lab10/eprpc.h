/***************************************************************************/
/* WARNING: DO NOT CHANGE. This file will be overwritten by Buildbot       */
/***************************************************************************/

#ifndef EPRPC_H
#define EPRPC_H

void eprpc_init(void);

void eprpc_load(const double num);

void eprpc_get_result(double* out_result);

void eprpc_add(void);
void eprpc_sub(void);
void eprpc_mul(void);
void eprpc_div(void);

#endif /* EPRPC_H */