#!/usr/bin/env bats

setup() {
    make spec
}

@test "ElaraSpec has 4 pended specs, and 29 other tests" {
    run ./ElaraSpec
    [ "$status" -eq 0 ]
    [ "$output" = "..**.**.........................." ]
}

@test "ElaraFocusedSpec has 3 skipped specs, and 4 other tests" {
    run ./ElaraFocusedSpec
    [ "$status" -eq 0 ]
    [ "$output" = "*.*...*" ]
}
