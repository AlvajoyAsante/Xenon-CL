# ----------------------------
# Program Options
# ----------------------------

NAME         ?= XENONCL
ICON         ?= icon.png
DESCRIPTION  ?= "XENON CL v0.0.27"
COMPRESSED   ?= YES
ARCHIVED     ?= NO

# ----------------------------

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

# ----------------------------

include $(CEDEV)/meta/makefile.mk