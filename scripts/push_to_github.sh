#!/usr/bin/env bash
set -euo pipefail

APPROVAL_TEXT="${1:-}"
EXPECTED_APPROVAL="APPROVE OS REPO"
REMOTE_URL="https://github.com/PeterNgabaRepo/operating-systems-circular-buffer.git"
REMOTE_NAME="origin"

if [[ "${APPROVAL_TEXT}" != "${EXPECTED_APPROVAL}" ]]; then
  echo "refusing to push: approval text must be exactly '${EXPECTED_APPROVAL}'" >&2
  exit 2
fi

if ! git status --short | grep -q '^'; then
  :
else
  echo "refusing to push: local repo has uncommitted changes" >&2
  git status --short >&2
  exit 1
fi

if ! git ls-remote "${REMOTE_URL}" >/dev/null 2>&1; then
  echo "refusing to push: target repo is not reachable at ${REMOTE_URL}" >&2
  echo "create the empty GitHub repo first, then rerun this script" >&2
  exit 1
fi

if git remote get-url "${REMOTE_NAME}" >/dev/null 2>&1; then
  current_remote="$(git remote get-url "${REMOTE_NAME}")"
  if [[ "${current_remote}" != "${REMOTE_URL}" ]]; then
    echo "refusing to push: origin points to ${current_remote}, expected ${REMOTE_URL}" >&2
    exit 1
  fi
else
  git remote add "${REMOTE_NAME}" "${REMOTE_URL}"
fi

current_branch="$(git branch --show-current)"
if [[ "${current_branch}" != "main" ]]; then
  git branch -M main
fi

git push -u "${REMOTE_NAME}" main
