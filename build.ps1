$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -nologo -utf8 -latest -property installationPath
Import-Module "${vsPath}\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath $vsPath -DevCmdArguments "-arch=amd64 -host_arch=amd64 -no_logo" -SkipAutomaticLocation

Set-Location $PSScriptRoot

$configuration = $args[0]
$platform = $args[1]

MSBuild.exe -property:"Configuration=${configuration};Platform=${platform}" -maxCpuCount -nologo ab_menu.sln
exit $LASTEXITCODE
