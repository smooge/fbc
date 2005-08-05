option explicit

defint a-z

dim as integer result

result = instr("d"+chr$(0), any chr$(0))
ASSERT( result = 2 )
result = instr("d"+chr$(0), any chr$(0)+"d")
ASSERT( result = 1 )
result = instr("d"+chr$(0), any "d"+chr$(0))
ASSERT( result = 1 )
result = instr("d"+chr$(0)+"x", any "x"+chr$(0))
ASSERT( result = 2 )
result = instr("d"+chr$(0)+"x", any "")
ASSERT( result = 1 )
result = instr("d"+chr$(0)+"x", any "q")
ASSERT( result = 0 )
result = instr("d"+chr$(0)+"x", any "qb")
ASSERT( result = 0 )

result = instr(2, "d"+chr$(0), any chr$(0))
ASSERT( result = 2 )
result = instr(2, "d"+chr$(0), any chr$(0)+"d")
ASSERT( result = 2 )
result = instr(2, "d"+chr$(0), any "d"+chr$(0))
ASSERT( result = 2 )
result = instr(2, "d"+chr$(0)+"x", any "x"+chr$(0))
ASSERT( result = 2 )
result = instr(2, "d"+chr$(0)+"x", any "")
ASSERT( result = 2 )
result = instr(2, "d"+chr$(0)+"x", any "q")
ASSERT( result = 0 )
result = instr(2, "d"+chr$(0)+"x", any "qb")
ASSERT( result = 0 )

result = instr(3, "d"+chr$(0)+"x", any "d"+chr$(0))
ASSERT( result = 0 )
