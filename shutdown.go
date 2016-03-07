/*
Package shutdown is a cross platfrom Go library to place an icon and menu in the notification area.
Supports Windows, Mac OSX and Linux currently.
Methods can be called from any goroutine except Run(), which should be called at the very beginning of main() to lock at main thread.
*/
package shutdown

import (
	"github.com/getlantern/golog"
)

var (
	log = golog.LoggerFor("shutdown")

	readyCh = make(chan interface{})
)

var (
	QueryEndSession = make(chan interface{})
	EndSession      = make(chan interface{})
)

// Run initializes GUI and starts the event loop, then invokes the onReady
// callback.
// It blocks until shutdown.Quit() is called.
// Should be called at the very beginning of main() to lock at main thread.
func Run(onReady func()) {
	go func() {
		<-readyCh
		onReady()
	}()

	nativeLoop()
}

// Quit the shutdown
func Quit() {
	quit()
}

func shutdownReady() {
	readyCh <- nil
}

func shutdownQueryEndSession() {
	QueryEndSession <- nil
}

func shutdownEndSession() {
	EndSession <- nil
}
