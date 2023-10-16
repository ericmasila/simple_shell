#!/usr/bin/env bash

set -e

SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOTDIR="$(git -C "$SCRIPTDIR" rev-parse --show-toplevel)"

set -x

cat > "${ROOTDIR}/AUTHORS" <<- EOF
  # This file lists all contributors to the repository.
  # To see how it's generated, visit https://github.com/moby/moby/blob/master/hack/generate-authors.sh

  $(git -C "$ROOTDIR" log --format='%aN <%aE>' | LC_ALL=C.UTF-8 sort -uf)

EOF

