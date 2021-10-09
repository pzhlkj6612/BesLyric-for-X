
param (
    [Parameter(Mandatory = $true)][string]$VERSION_NUMBER,
    [Parameter(Mandatory = $true)][string]$GIT_COMMIT,

    [Parameter(Mandatory = $true)][string]$GITHUB_ENV,

    [Parameter(Mandatory = $true)][string]$B4X_DEP_PATH,
    [Parameter(Mandatory = $true)][string]$DOWNLOAD_DIR_PATH,

    [Parameter(Mandatory = $true)][string]$TOOLCHAIN,
    [Parameter(Mandatory = $true)][switch]$SKIP_DOWNLOAD,
    [Parameter(Mandatory = $true)][switch]$SKIP_INNOSETUP,
    [Parameter(Mandatory = $true)][switch]$SKIP_EVB
)

# set env:
#   - PACKAGE_ZIP_FILE_PATH
#   - PACKAGE_INNOSETUP_FILE_PATH
#   - PACKAGE_EVB_FILE_PATH


$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3.0


$PSBoundParameters | Format-List


# https://stackoverflow.com/questions/39274324/get-total-number-of-cores-from-a-computer-without-hyperthreading
$threadNum = "$((Get-CimInstance -ClassName 'Win32_ComputerSystem').NumberOfLogicalProcessors)"


# https://stackoverflow.com/questions/34559553/create-a-temporary-directory-in-powershell/34559554#34559554
function New-TemporaryDirectory {
    $parent = [System.IO.Path]::GetTempPath()
    $name = [System.IO.Path]::GetRandomFileName()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}


# Prepare B4X_DEP_PATH

$B4X_DEP_INCLUDE_PATH_COMMON = "$B4X_DEP_PATH\include"
$B4X_DEP_INCLUDE_PATH_SDL2 = "$B4X_DEP_INCLUDE_PATH_COMMON\SDL2"
$B4X_DEP_LIB_PATH = "$B4X_DEP_PATH\lib"

New-Item -Force -ItemType 'Directory' -Path @(
    $B4X_DEP_PATH,
    $B4X_DEP_INCLUDE_PATH_COMMON,
    $B4X_DEP_INCLUDE_PATH_SDL2,
    $B4X_DEP_LIB_PATH,

    $DOWNLOAD_DIR_PATH
)


# Prepare directories

$repoDir = $PWD
$buildDir = $(New-TemporaryDirectory)
$installDir = $(New-TemporaryDirectory)
$deployDir = $(New-TemporaryDirectory)
$packageDir = $(New-TemporaryDirectory)
$cqtInstallDir = $(New-TemporaryDirectory)  # will be overwritten
$isInstallDir = $(New-TemporaryDirectory)
$evbInstallDir = $(New-TemporaryDirectory)


# ==============================================================
# ----- Get information about third-party library packages -----

function Config-FFmpeg {
    $name = 'FFmpeg'

    Write-Host "deploying $name"

    if ($SKIP_DOWNLOAD) { Write-Host 'skipped'; return; }

    Push-Location $(New-TemporaryDirectory)

        $devDir = 'ffmpeg-dev'
        $sharedDir = 'ffmpeg-shared'

        $devZip = "$devDir.zip"
        $sharedZip = "$sharedDir.zip"

        Write-Host "downloading $name"

        # https://archive.org/details/zeranoe
        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -OutFile $devZip -Uri 'https://archive.org/download/zeranoe/win64/dev/ffmpeg-4.3.1-win64-dev.zip'
        Invoke-WebRequest -OutFile $sharedZip -Uri 'https://archive.org/download/zeranoe/win64/shared/ffmpeg-4.3.1-win64-shared.zip'
        $ProgressPreference = 'Continue'

        Write-Host "extracting $name"

        Expand-Archive $devZip
        Expand-Archive $sharedZip

        Copy-Item "$sharedDir\ffmpeg-*\bin\*.dll" $B4X_DEP_LIB_PATH
        Copy-Item -Recurse "$devDir\ffmpeg-*\include\*" $B4X_DEP_INCLUDE_PATH_COMMON
        Copy-Item "$devDir\ffmpeg-*\lib\*.lib" $B4X_DEP_LIB_PATH

    Pop-Location
}

function Config-SDL2 {
    $name = 'SDL2'

    Write-Host "deploying $name"

    if ($SKIP_DOWNLOAD) { Write-Host 'skipped'; return; }

    Push-Location $(New-TemporaryDirectory)

        $packageDir = 'sdl2-dev'
        $packageZip = "$packageDir.zip"

        Write-Host "downloading $name"

        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -OutFile $packageZip -Uri 'https://www.libsdl.org/release/SDL2-devel-2.0.22-VC.zip'
        $ProgressPreference = 'Continue'

        Write-Host "extracting $name"

        Expand-Archive $packageZip

        Copy-Item @("$packageDir\SDL2-*\lib\x64\*.dll", "$packageDir\SDL2-*\lib\x64\*.lib") -Destination $B4X_DEP_LIB_PATH
        Copy-Item "$packageDir\SDL2-*\include\*.h" $B4X_DEP_INCLUDE_PATH_SDL2

    Pop-Location
}

function Config-OpenSSL {
    $name = 'OpenSSL'

    Write-Host "deploying $name"

    if ($SKIP_DOWNLOAD) { Write-Host 'skipped'; return; }

    Push-Location $(New-TemporaryDirectory)

        $packageDir = 'openssl-dev'
        $packageZip = "$packageDir.zip"

        Write-Host "downloading $name"

        # https://github.com/openssl/openssl/wiki/Binaries
        # https://wiki.overbyte.eu/wiki/index.php/ICS_Download#Download_OpenSSL_Binaries
        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -OutFile $packageZip -Uri 'https://web.archive.org/web/20231214082308if_/https://wiki.overbyte.eu/arch/openssl-1.1.1w-win64.zip'
        $ProgressPreference = 'Continue'

        Write-Host "extracting $name"

        Expand-Archive $packageZip

        Copy-Item "$packageDir\*.dll" -Destination $B4X_DEP_LIB_PATH

    Pop-Location
}

# ====================================================
# ----- Download third-party software installers -----

function Config-CQtDeployer {
    $name = 'CQtDeployer'

    Write-Host "deploying $name"

    Push-Location $DOWNLOAD_DIR_PATH

        $installer = 'cqtdeployer-installer.exe'

        if (-Not $SKIP_DOWNLOAD) {
            Write-Host "downloading $name"

            $ProgressPreference = 'SilentlyContinue'
            Invoke-WebRequest -OutFile $installer -Uri 'https://github.com/QuasarApp/CQtDeployer/releases/download/1.4.7/WindowsOfflineInstaller.exe'
            $ProgressPreference = 'Continue'
        }

        Write-Host "installing $name"

        $p = Start-Process -Wait -NoNewWindow -PassThru `
            -FilePath "$PWD\$installer" `
            -ArgumentList @(
                '--script', "$repoDir\.github\resources\qt-installer-noninteractive.qs",
                '--verbose',
                "targetDirectory=$cqtInstallDir"
            )
        If ($p.ExitCode -ne 0) { throw "exited with code $($p.ExitCode.toString())" }

        $cqtInstallDir = "$cqtInstallDir\1.4"

        Write-Host "testing $name"

        & "$cqtdeployerDir\cqtdeployer.bat" 'version'
        if ($LASTEXITCODE -ne 0) {
            throw "exited with code $LASTEXITCODE"
        }

    Pop-Location
}

function Config-InnoSetup {
    $name = 'Inno Setup'

    Write-Host "deploying $name"

    Push-Location $DOWNLOAD_DIR_PATH

        $installer = 'innosetup-installer.exe'

        if (-Not $SKIP_DOWNLOAD) {
            Write-Host "downloading $name"

            $ProgressPreference = 'SilentlyContinue'
            Invoke-WebRequest -OutFile $installer -Uri 'https://files.jrsoftware.org/is/6/innosetup-6.0.5.exe'
            $ProgressPreference = 'Continue'
        }

        Write-Host "installing $name"

        $p = Start-Process -Wait -NoNewWindow -PassThru `
            -FilePath "$PWD\$installer" `
            -ArgumentList @(
                '/VERYSILENT',
                "/DIR=$isInstallDir",
                '/CURRENTUSER'
            )
        If ($p.ExitCode -ne 0) { throw "exited with code $($p.ExitCode.toString())" }

        Write-Host "testing $name"

        # & 'ISCC.exe' '/?' # Ignore return value
        Start-Process -Wait -NoNewWindow -PassThru `
            -FilePath "$isInstallDir\ISCC.exe" `
            -ArgumentList @('/?')

    Pop-Location
}

function Config-EVB {
    $name = 'Enigma Virtual Box'

    Write-Host "deploying $name"

    Push-Location $DOWNLOAD_DIR_PATH

        $installer = 'evb-installer.exe'

        if (-Not $SKIP_DOWNLOAD) {
            Write-Host "downloading $name"

            $ProgressPreference = 'SilentlyContinue'
            Invoke-WebRequest -OutFile $installer -Uri 'https://enigmaprotector.com/assets/files/enigmavb.exe'
            $ProgressPreference = 'Continue'
        }

        Write-Host "installing $name"

        $p = Start-Process -Wait -NoNewWindow -PassThru `
            -FilePath "$PWD\$installer" `
            -ArgumentList @(
                '/VERYSILENT',
                "/DIR=$evbInstallDir",
                '/CURRENTUSER'
            )
        If ($p.ExitCode -ne 0) { throw "exited with code $($p.ExitCode.toString())" }

        Write-Host "testing $name"

        & "$evbInstallDir\enigmavbconsole.exe"
        if ($LASTEXITCODE -ne 0) {
            throw "exited with code $LASTEXITCODE"
        }

    Pop-Location
}

function Build-MSVCApp {
    $name = 'MSVC Build'

    Write-Host "cmake $name"

    & 'cmake.exe' @(
        , '-G', 'Visual Studio 17 2022'
        , '-A', 'x64'
        , '-D', "CMAKE_PREFIX_PATH=$B4X_DEP_PATH"
        , '-D', "CMAKE_INSTALL_PREFIX=$installDir"
        , '-D', "GIT_COMMIT_SHA1=$GIT_COMMIT"
        , '-D', "APP_VERSION=$VERSION_NUMBER"
        , '-S', $repoDir
        , '-B', $buildDir
    )
    if (${LASTEXITCODE} -ne 0) {
        throw "exited with code $LASTEXITCODE"
    }

    Write-Host "build $name"

    # https://stackoverflow.com/questions/39274324/get-total-number-of-cores-from-a-computer-without-hyperthreading
    & 'cmake.exe' @(
        , '--build', $buildDir
        , '-j', $threadNum
        , '--target', 'install'
        , '--config', 'RelWithDebInfo'
    )
    if (${LASTEXITCODE} -ne 0) {
        throw "exited with code $LASTEXITCODE"
    }
}

function Build-MinGWApp {
    $name = 'MinGW Build'

    Write-Host "cmake $name"

    & 'cmake.exe' @(
        , '-G', 'MinGW Makefiles'
        , '-D', 'CMAKE_BUILD_TYPE=RelWithDebInfo'
        , '-D', "CMAKE_PREFIX_PATH=$B4X_DEP_PATH"
        , '-D', "CMAKE_INSTALL_PREFIX=$installDir"
        , '-D', "GIT_COMMIT_SHA1=$GIT_COMMIT"
        , '-D', "APP_VERSION=$VERSION_NUMBER"
        , '-S', $repoDir
        , '-B', $buildDir
    )
    if (${LASTEXITCODE} -ne 0) {
        throw "exited with code $LASTEXITCODE"
    }

    Write-Host "build $name"

    & 'cmake.exe' @(
        , '--build', $buildDir
        , '-j', $threadNum
        , '--target', 'install'
    )
    if (${LASTEXITCODE} -ne 0) {
        throw "exited with code $LASTEXITCODE"
    }
}

function Build-App {
    if ($TOOLCHAIN -eq 'msvc') {
        Build-MSVCApp
    } elseif ($TOOLCHAIN -eq 'mingw') {
        Build-MinGWApp
    } else {
        throw "unknown toolchain: $TOOLCHAIN"
    }

    Get-ChildItem -Recurse -Path "$installDir" | FL
}

function Deploy-App {
    $name = 'windeployqt'

    Write-Host "deploy with $name"

    $windeployqtParams = @{
        WINDEPLOYQT_PATH   = 'windeployqt'
        LIB_DIR_PATH       = $B4X_DEP_LIB_PATH
        TARGET             = 'BesLyric-for-X'
        INSTALL_ROOT       = "$installDir"
        INCLUDE_PDB        = $false

        DEPLOY_DIR_PATH    = $deployDir
    }
    & "$repoDir\.deploy-package\Windows\deploy\call_windeployqt.ps1" @windeployqtParams
}

function Export-Env ([string]$NAME, [string]$VALUE) {
    "$NAME=$VALUE" | Out-File $GITHUB_ENV -Encoding utf8 -Append
}

# set env: PACKAGE_ZIP_FILE_PATH
function Package-Zip {
    $name = 'Zip'

    Write-Host "Package with $name"

    $zipPath = "$packageDir\BesLyric-for-X-$VERSION_NUMBER-Windows-portable.zip"

    $zipParams = @{
        Path = "$deployDir\*"
        CompressionLevel = 'Optimal'

        DestinationPath = $zipPath
    }
    Compress-Archive @zipParams

    Export-Env 'PACKAGE_ZIP_FILE_PATH' $zipPath
}

# set env: PACKAGE_INNOSETUP_FILE_PATH
function Package-InnoSetupApp {
    $name = 'Inno Setup'

    Write-Host "Package with $name"

    $isPath = "$packageDir\BesLyric-for-X-$VERSION_NUMBER-Windows-setup.exe"

    $isParams = @{
        ISCC_PATH               = "$isInstallDir\ISCC.exe"
        ISS_COMPRESSION         = 'lzma2/ultra64'
        TARGET                  = 'BesLyric-for-X'
        DEPLOY_DIR_PATH         = $deployDir

        ISS_INSTALLER_FILE_PATH = $isPath
    }
    & "$repoDir\.deploy-package\Windows\package\call_iscc.ps1" @isParams

    Export-Env 'PACKAGE_INNOSETUP_FILE_PATH' $isPath
}

# set env: PACKAGE_EVB_FILE_PATH
function Package-EVBApp {
    $name = 'Enigma Virtual Box'

    Write-Host "Package with $name"

    $evbPath = "$packageDir\BesLyric-for-X-$VERSION_NUMBER-Windows-allinone.exe"

    $evbParams = @{
        ENIGMAVBCONSOLE_PATH    = "$evbInstallDir\enigmavbconsole.exe"
        DOES_EVB_COMPRESS_FILES = $true
        TARGET                  = 'BesLyric-for-X'
        DEPLOY_DIR_PATH         = $deployDir

        EVB_BOX_FILE_PATH       = $evbPath
    }
    & "$repoDir\.deploy-package\Windows\package\call_evbconsole.ps1" @evbParams

    Export-Env 'PACKAGE_EVB_FILE_PATH' $evbPath
}


Config-FFmpeg

Config-SDL2

Config-OpenSSL

# Config-CQtDeployer

if (-Not $SKIP_INNOSETUP) {
    Config-InnoSetup
}
if (-Not $SKIP_EVB) {
    Config-EVB
}


Build-App


Deploy-App


Package-Zip

if (-Not $SKIP_INNOSETUP) {
    Package-InnoSetupApp
}

if (-Not $SKIP_EVB) {
    Package-EVBApp
}
