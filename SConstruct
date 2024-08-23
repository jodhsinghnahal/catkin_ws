# This is a SConstruct file to generate the xnet parameters database
# from the source spreadsheets, and to generate auto-generated
# code from that database

# Note: This file expects python to be in the workspace's virtual environment.
# Therefore, run scons with the --enable-virtualenv option or
# export SCONS_ENABLE_VIRTUALENV=1 in your program environment

from git import Repo
import time
import re
import os
from email.utils import formatdate

# A utility function to get the software version
# We're looking for the version string embedded in parentheses in the first
# line of the changelog, right after the package name.
# We just want the software version number.  There may be a package
# version (ie build number) after a dash.  We don't want the latter
# portion if it exists.
def fnGetVersion(changelog):
    version = ''
    with open(changelog, 'r') as fp:
        line = fp.readline()

    if line:
        match = re.search(r'\(([^-)]+)', line)
        if match:
            version = match.group(1)

    return version

# Action function to update the changelog with the current build number
# and date stamp.
# TODO: Are actual changes updated automatically from git or hand-entered?
# There's a bit of a chicken and egg problem here
def fnUpdateChangelog(target, source, env):
    changelog = str(target[0])
    lines = []
    with open(changelog, 'r') as fp:
        while True:
            line = fp.readline()
            if line:
                lines.append(line)
            else:
                break

    # The first line of the file has the package name, version, distribution and urgency.
    # The version can come in two parts: the software version and package version,
    # which are separated by a dash.  The whole version string is in parentheses.
    # We'll use the build number as the package version.
    newline = re.sub(r'\(.*\)', f'({env["VERSION"]}-{env["BUILD_NUMBER"]})', lines[0])
    if newline:
        lines[0] = newline

    # The last line of a changelog entry starts with
    # -- <name of maintainer> <email of maintainer>  <Date in RFC2822 format>
    for i, line in enumerate(lines):
        if line.startswith(' --'):
            newline = re.sub(r'  .*$', '  ' + formatdate(), line)
            lines[i] = newline
            break

    # Re-write the file
    with open(changelog, 'w') as fp:
        for line in lines:
            fp.write(line)

# Get the architecture for labeling the .deb package.  Default to 'all' if not found
def fnGetArchitecture():
    arch = 'all'
    ARCH_MATCHER = re.compile(r'^Architecture:\s*(\w+)')
    with open('dpkg/debian/control', 'r') as fp:
        while True:
            line = fp.readline()
            match = ARCH_MATCHER.match(line)
            if match:
                arch = match.group(1)
                break

    return arch

#--- Set up the build environment ---
# Tar builder creates tar.gz
env = Environment(TARFLAGS = '-c -z')
env['PROGNAME'] = 'Rvc2Mqtt'
env['PKGNAME'] = 'xgw-' + env['PROGNAME'].lower()
env['REPONAME'] = 'xantrex_xcp_rvc2mqtt'

# The package version comes from the top line of dpkg/debian/changelog
env['VERSION'] = fnGetVersion('dpkg/debian/changelog')
env['BUILD_NUMBER'] = os.environ['BUILD_NUMBER']

# The architecture from the dpkg/debian/control file
env['ARCHITECTURE'] = fnGetArchitecture()

# Carry through home directory so that emacs works for generating documents
env['ENV']['HOME'] = os.getenv('HOME')

# Carry through the NUITKA_CACHE_DIR environment variable for nuitka
env['ENV']['NUITKA_CACHE_DIR'] = os.getenv('NUITKA_CACHE_DIR')

# Capture the version information from the git repository
repo = Repo('.')
last_commit = repo.head.commit
env['GIT_VERSION'] = last_commit.hexsha
env['GIT_SHORT_VERSION'] = last_commit.hexsha[:7]

# Is the repo modified from what was last committed?
env['REPO_CLEAN'] = not repo.is_dirty()

# Is the last commit the same as what is on Bitbucket?
synced = False
origin_master = repo.remotes['origin'].refs['master']
if origin_master.commit == last_commit:
    synced = True

env['MASTER_SYNCED'] = synced

# Get the build date and time
env['BUILD_DATE'] = time.strftime('%Y-%m-%d %H:%M:%S %Z')
env['YEAR'] = time.strftime('%Y')

#--- Use that build environment to build things ---

# Build the database that the program and documentation depend on
database = env.Command('xnet_params.db',
                       Glob('docs/*.xlsx') + ['MakeParamDatabase.py', 'MakeAlertTable.py'],
                       'python MakeParamDatabase.py && python MakeAlertTable.py')

# Generate code from the database
gencode = env.Command('XnetParams.py',
                      database,
                      'python ParamListGen.py')

# Generate a version file from template
env['SUBST_DICT'] = {'{{ PROGNAME }}' : '$PROGNAME',
                     '{{ REPONAME }}' : '$REPONAME',
                     '{{ YEAR }}' : '$YEAR',
                     '{{ BUILD_DATE }}' : '$BUILD_DATE',
                     '{{ VERSION }}' : '$VERSION',
                     '{{ GIT_VERSION }}' : '$GIT_SHORT_VERSION',
                     '{{ MASTER_SYNCED }}' : '$MASTER_SYNCED',
                     '{{ REPO_CLEAN }}' : '$REPO_CLEAN'}
vercode = env.Substfile('VersionInfo.py.in')
env.AlwaysBuild(vercode)

# Also create a build information text file that can be tucked into the tarball
buildinfo_doc = env.Substfile('BUILDINFO.txt.in')
env.AlwaysBuild(buildinfo_doc)

# Create an updated changelog for the Debian package
changelog = env.Command('dpkg/debian/changelog',
                        None,
                        fnUpdateChangelog)
env.AlwaysBuild(changelog)
env.Precious(changelog)
env.NoClean(changelog)
Alias('changelog', changelog)

# Generate documentation from the database, using emacs and org-mode
docs = env.Command('docs/product_support.rst',
                   database + ['docs/product_support.org', 'docs/update_product_support.el'],
                   './update_product_support.el',
                   chdir=1)

# The product_support.rst file goes directly into the global documentation package
global_docs = env.Install('/home/pi/xantrex_xcp_global_docs/docs/source', docs)
Alias("GlobalDocs", global_docs)

# Build the binary with Nuitka
binary = env.Command('${PROGNAME}.dist/$PROGNAME',
                     ['${PROGNAME}.py', 'DiagMsgHandler.py', 'BmsFlags2Alerts.py'] + gencode + vercode,
                     'python -m nuitka ${PROGNAME}.py --include-plugin-directory=venv --include-module=_cffi_backend --standalone')

Alias('Binary', binary)

complete_binary = env.Install('${PROGNAME}.dist', buildinfo_doc + ["logging.yaml"])
Depends(complete_binary, binary)

# Build a tarball from the contents of Rvc2Mqtt.dist
tarball = env.Tar('${PROGNAME}_${GIT_SHORT_VERSION}.tar.gz', '${PROGNAME}.dist/')
Depends(tarball, complete_binary)
Alias('Tarball', tarball)

# Build the Debian package
dpkg = env.Command('${PKGNAME}_${VERSION}-${BUILD_NUMBER}_${ARCHITECTURE}.deb',
                   complete_binary,
                   'dpkg-buildpackage --build=binary',
                   chdir='dpkg')
Depends(dpkg, changelog)
Alias('Deb', dpkg)

