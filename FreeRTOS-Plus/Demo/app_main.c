/* -----------------------------------------------------------------------------
 * Copyright (c) 2021-2026 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdarg.h>
#include "cmsis_os2.h"

extern int32_t socket_startup (void);
extern void vStartSimpleMQTTDemo (void);

static const osThreadAttr_t app_main_attr = {
  .stack_size = 4096U
};

void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list args;

    va_start( args, pcFormat );
    vprintf( pcFormat, args );
    va_end( args );
}

/*---------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
static void app_main_thread (void *argument) {
  int32_t status;

  (void)argument;

  status = socket_startup();

  if (status == 0) {
    /* Start demo task */
    vStartSimpleMQTTDemo();
  }

  osDelay(osWaitForever);
  for (;;) {}
}

/*---------------------------------------------------------------------------
 * Application initialization
 *---------------------------------------------------------------------------*/
int app_main (void) {

  osKernelInitialize();

  osThreadNew(app_main_thread, NULL, &app_main_attr);

  osKernelStart();
  return 0;
}
