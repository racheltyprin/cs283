#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#!/usr/bin/env bats

# Test if dsh runs basic commands
@test "Check if dsh runs ls without errors" {
    run ./dsh <<EOF                
ls
EOF
    [ "$status" -eq 0 ]
}

# Test if dsh handles empty input gracefully
@test "Check if dsh handles empty input" {
    run ./dsh <<EOF                

EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"warning: no commands provided"* ]]
}

# Test if dsh handles exit command correctly
@test "Check if dsh exits cleanly" {
    run ./dsh <<EOF                
exit
EOF
    [ "$status" -eq 0 ]
}

# Test if dsh can change directories (cd command)
@test "Check if dsh executes cd command" {
    run ./dsh <<EOF                
cd /
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"/"* ]]
}

# Test if dsh executes an invalid command properly
@test "Check if dsh handles invalid commands" {
    run ./dsh <<EOF                
invalidcommand
EOF
    echo "DEBUG: status = $status, output = $output"  # Add this for debugging
    [ "$status" -ne -6 ]
    [[ "$output" == *"command not found"* ]]
}

# Test if dsh executes a sequence of commands
@test "Check if dsh handles multiple commands" {
    run ./dsh <<EOF                
echo test
echo another test
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"test"* ]]
    [[ "$output" == *"another test"* ]]
}
