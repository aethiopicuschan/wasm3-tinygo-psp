package main

import "unsafe"

//go:wasmimport debug println
func println(ptr unsafe.Pointer, len int32)

//export start
func start() {
	main()
}

func main() {
	message := "Hello, WebAssembly, from Golang!"
	println(unsafe.Pointer(unsafe.StringData(message)), int32(len(message)))
}
