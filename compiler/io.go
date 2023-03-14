/*
 *   Copyright (C) 2022-2023 Barn-Lang Organization
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

package main

import (
	"syscall"
	"bytes"
	"os/exec"
)

func run_command(name string, args ...string) (stdout string, stderr string, exit_code int) {
    var outbuf, errbuf bytes.Buffer
    cmd := exec.Command(name, args...)
    cmd.Stdout = &outbuf
    cmd.Stderr = &errbuf

    err := cmd.Run()
    stdout = outbuf.String()
    stderr = errbuf.String()

    if err != nil {
        if exitError, ok := err.(*exec.ExitError); ok {
            ws := exitError.Sys().(syscall.WaitStatus)
            exit_code = ws.ExitStatus()
        } else {
            exit_code = 1
            if stderr == "" {
                stderr = err.Error()
            }
        }
    } else {
        ws := cmd.ProcessState.Sys().(syscall.WaitStatus)
        exit_code = ws.ExitStatus()
    }
    return
}