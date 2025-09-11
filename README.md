# LIRS Cache

## Algorithm
`LIRS` (Low Inter-Reference Recency Set) — множество с низкой референсной давностью, то есть элементы, которые часто используются.  
`HIRS` (High Inter-Reference Recency Set) — множество с высокой референсной давностью, то есть элементы, которые реже используются в кэше.  
<table>
    <tr>
        <td><img src="imgs/get.png" width="90%" height="auto"></td>
        <td><img src="imgs/put.png" width="90%" height="auto"></td>
    </tr>
</table>

## Performance
![graph](imgs/LIRS_Cache_Performance.png)