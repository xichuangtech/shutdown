package shutdown

import (
	//	"fmt"
	"os"
	"path/filepath"
	"syscall"

	//	"github.com/getlantern/filepersist"
)

var (
	dllDir  = filepath.Join(os.Getenv("APPDATA"), "shutdown")
	dllFile = "shutdown.dll"

	mod         = syscall.NewLazyDLL(dllFile)
	_nativeLoop = mod.NewProc("nativeLoop")
	_quit       = mod.NewProc("quit")
)

func nativeLoop() {
	_nativeLoop.Call(
		syscall.NewCallbackCDecl(shutdown_ready),
		syscall.NewCallbackCDecl(shutdown_query_endsession),
		syscall.NewCallbackCDecl(shutdown_endsession))
}

func quit() {
	_quit.Call()
}

// shutdown_ready takes an ignored parameter just so we can compile a callback
// (for some reason in Go 1.4.x, syscall.NewCallback panics if there's no
// parameter to the function).
func shutdown_ready(ignore uintptr) uintptr {
	shutdownReady()
	return 0
}

func shutdown_query_endsession() uintptr {
	shutdownQueryEndSession()
	return 0
}

func shutdown_endsession() uintptr {
	shutdownEndSession()
	return 0
}
