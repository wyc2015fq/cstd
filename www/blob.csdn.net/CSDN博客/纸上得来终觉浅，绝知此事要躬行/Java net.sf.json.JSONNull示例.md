# Java net.sf.json.JSONNull示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月02日 12:53:56[boonya](https://me.csdn.net/boonya)阅读数：39
原文地址：[https://www.programcreek.com/java-api-examples/index.php?api=net.sf.json.JSONNull](https://www.programcreek.com/java-api-examples/index.php?api=net.sf.json.JSONNull)
以下是使用net.sf.json.JSONNull的示例。
示例1：
```java
/**
 * {@inheritDoc}
 */
@Override
protected void execute() {
    JSONObject postDifferentialCommentResult = postDifferentialComment(comment, SILENT,
            commentAction);
    if (postDifferentialCommentResult == null ||
            !(postDifferentialCommentResult.get("error_info") instanceof JSONNull)) {
        if (postDifferentialCommentResult != null) {
            info(String.format("Got error %s with action %s",
                    postDifferentialCommentResult.get("error_info"), commentAction));
        }
        info("Re-trying with action 'none'");
        postDifferentialComment(comment, SILENT, DEFAULT_COMMENT_ACTION);
    }
}
```
示例2：
```java
public CodeCoverageMetrics getParentCoverage(String sha) {
    if (sha == null) {
        return null;
    }
    try {
        String coverageJSON = getCoverage(sha);
        JsonSlurper jsonParser = new JsonSlurper();
        JSON responseJSON = jsonParser.parseText(coverageJSON);
        if (responseJSON instanceof JSONNull) {
            return null;
        }
        JSONObject coverage = (JSONObject) responseJSON;
        return new CodeCoverageMetrics(
                ((Double) coverage.getDouble(PACKAGE_COVERAGE_KEY)).floatValue(),
                ((Double) coverage.getDouble(FILES_COVERAGE_KEY)).floatValue(),
                ((Double) coverage.getDouble(CLASSES_COVERAGE_KEY)).floatValue(),
                ((Double) coverage.getDouble(METHOD_COVERAGE_KEY)).floatValue(),
                ((Double) coverage.getDouble(LINE_COVERAGE_KEY)).floatValue(),
                ((Double) coverage.getDouble(CONDITIONAL_COVERAGE_KEY)).floatValue());
    } catch (Exception e) {
        e.printStackTrace(logger.getStream());
    }
    return null;
}
```
示例3：
```
public void test_for_issue() throws Exception {
        Model model = new Model();
        model.object = JSONNull.getInstance();
        System.out.println(JSON.toJSONString(model));
//        System.out.println(JSON.toJSONString(map));
    }
```
示例4：
```java
/**
 * Execute Http request and response code
 * @param request - HTTP Request
 * @param expectedCode - expected response code
 * @return - response in JSONObject
 */
public JSON query(HttpRequestBase request, int expectedCode) throws IOException {
    log.info("Requesting: " + request);
    addRequiredHeader(request);
    HttpParams requestParams = request.getParams();
    requestParams.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, TIMEOUT * 1000);
    requestParams.setIntParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, TIMEOUT * 1000);
    synchronized (httpClient) {
        String response;
        try {
            HttpResponse result = httpClient.execute(request);
            int statusCode = result.getStatusLine().getStatusCode();
            response = getResponseEntity(result);
            if (statusCode != expectedCode) {
                notifier.notifyAbout("Response with code " + statusCode + ": " + extractErrorMessage(response));
                throw new IOException("API responded with wrong status code: " + statusCode);
            } else {
                log.debug("Response: " + response);
            }
        } finally {
            request.abort();
        }
        if (response == null || response.isEmpty()) {
            return JSONNull.getInstance();
        } else {
            return JSONSerializer.toJSON(response, new JsonConfig());
        }
    }
}
```
示例5：
```
/**
 * Try to send a message to harbormaster
 * @param unitResults the unit testing results to send
 * @param coverage the coverage data to send
 * @return false if an error was encountered
 */
private boolean sendMessage(UnitResults unitResults, Map<String, String> coverage, LintResults lintResults) throws IOException, ConduitAPIException {
    JSONObject result = diffClient.sendHarbormasterMessage(phid, harbormasterSuccess, unitResults, coverage, lintResults);
    if (result.containsKey("error_info") && !(result.get("error_info") instanceof JSONNull)) {
        info(String.format("Error from Harbormaster: %s", result.getString("error_info")));
        failTask();
        return false;
    } else {
        this.result = Result.SUCCESS;
    }
    return true;
}
```
示例6：
```java
/**
 * Return the local branch name
 *
 * @return the name of the branch, or unknown
 */
public String getBranch() {
    Object branchName = rawJSON.get("branch");
    if (branchName instanceof JSONNull) {
        return "(none)";
    }
    try {
        return (String) branchName;
    } catch (ClassCastException e) {
        return "(unknown)";
    }
}
```
示例7：
```java
@Test
public void testGetBranchWithEmptyResponse() throws Exception {
    JSONObject empty = new JSONObject();
    empty.put("branch", JSONNull.getInstance());
    Differential diff = new Differential(empty);
    assertEquals("(none)", diff.getBranch());
}
```
示例8：
```java
public final MapTuple put(final String field, final @Nullable Object obj) {
  if (obj == null) {
    /*
     * We need this because of joins.  We don't use Java null because
     * too many libraries either return null to represent absence,
     * don't allow null entries, or use null to represent something else.
     */
    _values.put(field, JSONNull.getInstance());
  } else {
    _values.put(field, obj);
  }
  return this;  // for chaining
}
```
示例9：
```java
@SuppressWarnings("unchecked")
public static void removeNulls(JSONObject json) {
  Iterator<String> keyIter = json.keys();
  while(keyIter.hasNext()) {
    String key = keyIter.next().toString();
    if (json.get(key) instanceof JSONNull || json.get(key) == null) {
      json.remove(key);
    }
  }
}
```
示例10：
```java
public static JSONObject parseObj(String o) {
  JSON j = JSONSerializer.toJSON(o);
  if (j instanceof JSONNull) {
    return null;
  } else {
    return (JSONObject)j;
  }
}
```
示例11：
```java
public static JSONArray parseArray(String o) {
  JSON j = JSONSerializer.toJSON(o);
  if (j instanceof JSONNull) {
    return null;
  } else {
    return (JSONArray)j;
  }
}
```
示例12：
```java
public static String getStringForType(Object value) {
  if(value instanceof String) {
    return "\""+ StringEscapeUtils.escapeJson((String)value) +"\"";
  } else if(value instanceof Integer || value instanceof Float || value instanceof Long || value instanceof Boolean) {
    return value.toString();
  } else if(value instanceof JSONArray) {
    return JSONUtil.toString((JSONArray) value);
  } else if(value instanceof JSONObject) {
    return JSONUtil.toString((JSONObject) value);
  } else if(value instanceof JSONNull || value == null) {
    return "null";
  } else {
    throw new RuntimeException("unrecognized type for value: "+value+"["+value.getClass().getName()+"]");
  }
}
```
示例13：
```java
@Override
public boolean configure(StaplerRequest req, JSONObject formData) throws FormException {
	// To persist global configuration information,
	// set that to properties and call save().
	Object identities = formData.get("chefIdentity");
	if (!JSONNull.getInstance().equals(identities)) {
		chefIdentities = req.bindJSONToList(ChefIdentity.class, identities);
	} else {
		chefIdentities = null;
	}
	save();
	return super.configure(req,formData);
}
```
示例14：
```java
@Override
public boolean configure(StaplerRequest req, JSONObject json) throws FormException {
	Object s = json.get("servers");
	if (!JSONNull.getInstance().equals(s)) {
		servers = req.bindJSONToList(Server.class, s);
	} else {
		servers = null;
	}
	publicKeyPath = json.getString("publicKeyPath");
	save();
	return super.configure(req, json);
}
```
示例15：
```java
@SuppressWarnings("rawtypes")
public Vertex convertElement(Object json, Network network) {
	try {
		if (json == null) {
			return null;
		}
		Vertex object = null;
		if (json instanceof JSONObject) {
			object = network.createVertex();
			for (Iterator iterator = ((JSONObject)json).keys(); iterator.hasNext(); ) {
				String name = (String)iterator.next();
				Object value = ((JSONObject)json).get(name);
				if (value == null) {
					continue;
				}
				Primitive key = new Primitive(name);
				Vertex target = convertElement(value, network);
				object.addRelationship(key, target);
			}
		} else if (json instanceof JSONArray) {
			object = network.createInstance(Primitive.ARRAY);
			JSONArray array = (JSONArray)json;
			for (int index = 0; index < array.size(); index++) {
				Vertex element = convertElement(array.get(index), network);
				object.addRelationship(Primitive.ELEMENT, element, index);
			}
		} else if (json instanceof JSONNull) {
			object = network.createInstance(Primitive.NULL);
		} else if (json instanceof String) {
			object = network.createVertex(json);
		} else if (json instanceof Number) {
			object = network.createVertex(json);
		} else if (json instanceof Date) {
			object = network.createVertex(json);
		} else if (json instanceof Boolean) {
			object = network.createVertex(json);
		} else {
			log("Unknown JSON object", Level.INFO, json);
		}
		return object;
	} catch (Exception exception) {
		log(exception);
		return null;
	}
}
```
示例16：
```java
/**
 * Initialises the value tuple.
 *
 * @param type
 * @param value
 */
public ValueTuple(String type, Object value) {
    this.type = type;
    this.value = (value != null ? value : JSONNull.getInstance());
}
```
