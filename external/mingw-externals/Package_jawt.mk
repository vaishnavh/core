# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# This file is part of the LibreOffice project.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

$(eval $(call gb_Package_Package,jawt,$(call gb_CustomTarget_get_workdir,external/jawt)))

$(error FIXME do not deliver this stuff to OUTDIR)

$(eval $(call gb_Package_add_file,jawt,lib/libjawt.dll.a,libjawt.dll.a))

# vim:set shiftwidth=4 tabstop=4 noexpandtab:
