/* 
 * File:   badge_apps.h
 * Author: morgan
 *
 * Created on March 4, 2017, 6:33 PM
 */

// Create one header that declares everything needed to create/use
// badge application tasks

#ifndef BADGE_APPS_H
#define	BADGE_APPS_H

// Declare tasks
void hello_world_task(void* p_arg);

void badgey_bird_task(void* p_arg);

void badgelandia_task(void* p_arg);

void groundwar_task(void* p_arg);

void screensaver_task(void* p_arg);

void boot_splash_task(void *p_arg);

void blinkenlights_task(void *args);

void conductor_task(void *args);

void dice_roll_task(void* p_arg);
#endif

