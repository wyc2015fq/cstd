# hbase的limit实现 - zj360202的专栏 - CSDN博客





2013年03月20日 17:44:36[zj360202](https://me.csdn.net/zj360202)阅读数：3026







`scan 'fct_pf_vs_hour',{LIMIT=>5}`

大家可以修改一下hbase中hbase-0.92.1-cdh4.1.2\src\main\ruby\hbase\table.rb中的scan方法来实现类似分页的操作；

随便贴下scan方法的ruby代码

```
def scan(args = {})
      unless args.kind_of?(Hash)
        raise ArgumentError, "Arguments should be a hash. Failed to parse #{args.inspect}, #{args.class}"
      end

      limit = args.delete("LIMIT") || -1
      maxlength = args.delete("MAXLENGTH") || -1

      if args.any?
        filter = args["FILTER"]
        startrow = args["STARTROW"] || ''
        stoprow = args["STOPROW"]
        timestamp = args["TIMESTAMP"]
        columns = args["COLUMNS"] || args["COLUMN"] || get_all_columns
        cache_blocks = args["CACHE_BLOCKS"] || true
        cache = args["CACHE"] || 0
        versions = args["VERSIONS"] || 1
        timerange = args[TIMERANGE]

        # Normalize column names
        columns = [columns] if columns.class == String
        unless columns.kind_of?(Array)
          raise ArgumentError.new("COLUMNS must be specified as a String or an Array")
        end

        scan = if stoprow
          org.apache.hadoop.hbase.client.Scan.new(startrow.to_java_bytes, stoprow.to_java_bytes)
        else
          org.apache.hadoop.hbase.client.Scan.new(startrow.to_java_bytes)
        end

        columns.each do |c| 
          family, qualifier = parse_column_name(c.to_s)
          if qualifier
            scan.addColumn(family, qualifier)
          else
            scan.addFamily(family)
          end
        end

        unless filter.class == String
          scan.setFilter(filter)
        else
          scan.setFilter(org.apache.hadoop.hbase.filter.ParseFilter.new.parseFilterString(filter))
        end

        scan.setTimeStamp(timestamp) if timestamp
        scan.setCacheBlocks(cache_blocks)
        scan.setCaching(cache) if cache > 0
        scan.setMaxVersions(versions) if versions > 1
        scan.setTimeRange(timerange[0], timerange[1]) if timerange
      else
        scan = org.apache.hadoop.hbase.client.Scan.new
      end

      # Start the scanner
      scanner = @table.getScanner(scan)
      count = 0
      res = {}
      iter = scanner.iterator

      # Iterate results
      while iter.hasNext
        if limit > 0 && count >= limit
          break
        end

        row = iter.next
        key = org.apache.hadoop.hbase.util.Bytes::toStringBinary(row.getRow)

        row.list.each do |kv|
          family = String.from_java_bytes(kv.getFamily)
          qualifier = org.apache.hadoop.hbase.util.Bytes::toStringBinary(kv.getQualifier)

          column = "#{family}:#{qualifier}"
          cell = to_string(column, kv, maxlength)

          if block_given?
            yield(key, "column=#{column}, #{cell}")
          else
            res[key] ||= {}
            res[key][column] = cell
          end
        end

        # One more row processed
        count += 1
      end

      return ((block_given?) ? count : res)
    end
```






