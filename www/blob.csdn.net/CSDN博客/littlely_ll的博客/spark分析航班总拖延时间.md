# spark分析航班总拖延时间 - littlely_ll的博客 - CSDN博客





2017年06月11日 22:04:15[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：360








```python
import csv
import matplotlib.pyplot as plt

from StringIO import StringIO
from datetime import datetime
from collections import namedtuple
from operator import add, itemgetter
from pyspark import SparkConf, SparkContext

APP_NAME = "Flight Delay Analysis"
DATE_FMT = "%Y-%m-%d"
TIME_FMT = "%H%M"

fields = ('date','airline','flightnum','origin','dest','dep',\
          'dep_delay','arv','arv_delay','airtime','distance')
Flight = namedtuple("Flight",fields)

##closure functions
def parse(row):
    '''
    Parse a row and returns a named tuple
    :param row:
    :return:
    '''
    row[0] = datetime.strptime(row[0], DATE_FMT).date()
    row[5] = datetime.strptime(row[5], TIME_FMT).time()
    row[6] = float(row[6])
    row[7] = datetime.strptime(row[7],TIME_FMT).time()
    row[8] = float(row[8])
    row[9] = float(row[9])
    row[10] = float(row[10])
    return Flight(*row[:11])

def split(line):
    '''
    Operator function for splitting a line with csv module
    :param line:
    :return:
    '''
    reader = csv.reader(StringIO(line))
    return reader.next()

def plot(delays):
    '''
    Show a bar chart of the total delay per airline
    :param depays:
    :return:
    '''
    airlines = [d[0] for d in delays]
    minutes = [d[1] for d in delays]
    index = list(xrange(len(airlines)))
    fig, axe = plt.subplots()
    bars = axe.barh(index, minutes)
    # Add the total minutes to the right
    for idx, air, min in zip(index, airlines, minutes):
        if min > 0:
            bars[idx].set_color('#d9230f')
            axe.annotate("%0.0f min" % min, xy=(min+1,idx+0.5),va = 'center')
        else:
            bars[idx].set_color("#469408")
            axe.annotate("%0.0f min" % min, xy=(10,idx+0.5),va='center')
    #Set the ticks
    ticks = plt.yticks([idx+0.5 for idx in index], airlines)
    xt = plt.xticks()[0]
    plt.xticks(xt, [' ']*len(xt))

    # Minimize chart junk
    plt.grid(axis='x',color='white',linestyle='-')
    plt.title('Total Minutes Delayed per Airline')
    plt.show()

def main(sc):
    '''
    Describe the trnasformations and actions used on the dataset, then
    plot the visualization on the output using matplotlib.
    :param sc:
    :return:
    '''
    ## Load the airlines lookup dictionary
    airlines = dict(sc.textFile("/test/airlines.csv").map(split).collect())

    ## Broadcast the lookup dictionary to cluster
    airline_lookup = sc.broadcast(airlines)

    ## Read the csv data into an RDD
    flights = sc.textFile("/test/flights.csv").map(split).map(parse)

    # Map the total delay to the airline (joined using the broadcast value)
    delays = flights.map(lambda f: (airline_lookup.value[f.airline],add(f.dep_delay, f.arv_delay)))

    # Reduce the total delay for the month to the airline
    delays = delays.reduceByKey(add).collect()
    delays = sorted(delays, key=itemgetter(1))

    ## Provide output from the driver
    for d in delays:
        print "%0.0f minutes delayed\t%s" % (d[1],d[0])

    ##Show a bar chart of the delays
    plot(delays)

if __name__ == "__main__":
    ##Configure Spark
    conf = SparkConf().setMaster("local[*]")
    conf = conf.setAppName(APP_NAME)
    sc = SparkContext(conf = conf)
    ## Execute the main functionality
    main(sc)
```

![这里写图片描述](https://img-blog.csdn.net/20170611220256141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



