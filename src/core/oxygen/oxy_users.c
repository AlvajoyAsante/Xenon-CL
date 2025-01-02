#include "oxy_users.h"

#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>

struct oxy_user_t *oxy_user;
struct oxy_user_system_t oxy_user_system;


uint8_t oxy_NewUser(char *name, char *password, const uint8_t type)
{
	struct oxy_user_t *curr_user;

	oxy_user = realloc(oxy_user, ++oxy_user_system.user_amount * sizeof(struct oxy_user_t));
	curr_user = &oxy_user[oxy_user_system.user_amount - 1];

	curr_user->user_id = oxy_user_system.user_amount - 1;
	curr_user->icon = NULL;

	/* username and password */
	if (name != NULL) {
		oxy_SetUserName(name, curr_user->user_id);
	}else{
		curr_user->name[0] = '\0';
	}

	if (password != NULL) {
		oxy_SetUserPassword(password, curr_user->user_id);
	}else{
		curr_user->password[0] = '\0';
	}

	/* type and login data */
	curr_user->type = type;
	curr_user->login_time[0] = 0;
	return oxy_user_system.user_amount;
}

void oxy_SetUserPassword(char *password, const int index)
{
	strncpy(oxy_user[index].password, password, sizeof(oxy_user[index].password));
}

void oxy_SetUserName(char *name, const int index)
{
	strncpy(oxy_user[index].name, name, sizeof(oxy_user[index].name));
}

void oxy_UserSetIcon(gfx_sprite_t *icon, const int index)
{
	oxy_user[index].icon = icon;
}

void oxy_DeleteUser(int index)
{
	if (oxy_user_system.user_amount > 1)
	{
		oxy_user[index] = oxy_user[oxy_user_system.user_amount - 1];
		oxy_user = realloc(oxy_user, oxy_user_system.user_amount-- * sizeof(struct oxy_user_t));
	}
}

bool oxy_UserCheckPassword(const char *input, const int index)
{
	return (bool)(!strcmp(oxy_user[index].password, input));
}

/*void oxy_UserSetLoginTime(const int index)
{
	oxy_user[index].login_time[0] = month;
	oxy_user[index].login_time[1] = day;

	if (oxy_user[index].login_time[3] != day) {
		oxy_user[index].login_time[2] = 1;
		oxy_user[index].login_time[3] = day;
	}else{
		oxy_user[index].login_time[2]++;
	}
}*/