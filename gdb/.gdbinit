define phead
    set $ptr = $arg1
    plistdata $arg0
end

define pnext
    set $ptr = $ptr->next
    plistdata $arg0
end

define plistdata
    if $ptr
        set $pdata = $ptr->data
    else
        set $pdata = 0
    end
    if $pdata
        p ($arg0 *)$pdata
    else
        p "NULL"
    end
end
