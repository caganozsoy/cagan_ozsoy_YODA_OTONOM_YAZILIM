from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
from sentence_transformers import SentenceTransformer, util
import time
import numpy as np
from sklearn.ensemble import RandomForestClassifier


X_train = np.array([
    [0.95, 0.98, 1], [0.92, 0.90, 1], [0.88, 0.85, 0],
    [0.40, 0.30, 0], [0.20, 0.50, 0], [0.75, 0.15, 0],
    [0.90, 0.10, 0], [0.30, 0.95, 0]
    # 1 = Same, 0 = Not Same
])

y_train = np.array([1, 1, 0, 0, 0, 0, 0, 0])

rf_model = RandomForestClassifier(n_estimators=50, random_state=42)
rf_model.fit(X_train, y_train)

bert_model = SentenceTransformer('dbmdz/bert-base-turkish-cased')


def clean_price(price_tag):
    # I cleaned specific symbols for making price tag strings same
    try:
        clean_tag = price_tag.replace("TL", "").replace(
            ".", "").replace(",", ".").replace(" ", "").strip()
        return float(clean_tag)
    except ValueError:
        return 0.0


def autonomous_data_pull(driver, url):
    # For autonomous actions using Selenium
    driver.get(url)
    wait = WebDriverWait(driver, 15)
    time.sleep(5)  # waiting for site to download itself
    try:
        try:
            name_element = wait.until(
                EC.presence_of_element_located((By.ID, "productTitle")))
        except:
            name_element = wait.until(
                EC.presence_of_element_located((By.TAG_NAME, "h1")))

        raw_name = driver.execute_script(
            "return arguments[0].innerText;", name_element).strip()

        name = raw_name.split('\n')[0].strip()

    except TimeoutException:
        name = "Unknown Product"

    price = "0"
    price_elements = driver.find_elements(
        By.XPATH, "//*[contains(@class, 'price') or contains(@class, 'fiyat') or contains(@class, 'prc')]")

    # For some Turkish e-commerce sites I used these price selectors
    possible_classWebsites = [
        "span.prc-dsc",  # trendyol price tag
        "span.discounted",  # for trendyol discount tags
        "div.pt_v8",  # akakce code
        "span.price__item",  # general code
        "span.a-price-whole",  # For amazon
    ]
    for css_selector in possible_classWebsites:
        try:
            firsttag = driver.find_element(By.CSS_SELECTOR, css_selector)
            price_text = firsttag.get_attribute("textContent").strip()
            if any(c.isdigit() for c in price_text):
                price = price_text
                break
        except Exception:
            # element not found, trying next detector
            continue

    if price == "0":
        # Applying plan B if price is still "0"
        for n in price_elements:
            try:
                text = n.text
                min_length = 2
                max_length = 16
                # Trying to detect the price tag
                if any(char.isdigit() for char in text) and min_length < len(text) < max_length:
                    price = text
                    break  # breaks after finding first price tag
            except Exception:
                # Same operation applied with css_selector
                continue

    return name, price


driver = webdriver.Safari()
print("\nSales Bot Autonomous System")
link1 = input("1. product URL: ")
link2 = input("2. product URL: ")


try:
    # Pulling datas(name and price) from the autonomous function
    product_name1, price1_tag = autonomous_data_pull(driver, link1)
    product_name2, price2_tag = autonomous_data_pull(driver, link2)

    # BERT usage
    emb1system = bert_model.encode(product_name1, convert_to_tensor=True)
    emb2system = bert_model.encode(product_name2, convert_to_tensor=True)
    # Usage of Cosine Similarity
    score = util.cos_sim(emb1system, emb2system).item()

    # I used the clean function for comparison
    priceTag1_num = clean_price(price1_tag)
    priceTag2_num = clean_price(price2_tag)

    print("\n")
    print(f"Product 1: {product_name1} | Price: {priceTag1_num} TL")
    print(f"Product 2: {product_name2} | Price: {priceTag2_num} TL")
    print(f"Similarity Score: %{score*100:.2f}")
    print("\n")

    if max(priceTag1_num, priceTag2_num) > 0:
        price_ratio = min(priceTag1_num, priceTag2_num) / \
            max(priceTag1_num, priceTag2_num)
    else:
        price_ratio = 0

    brand1 = product_name1.split()[0].lower()
    brand2 = product_name2.split()[0].lower()
    if brand1 == brand2:
        brand_match = 1
    else:
        brand_match = 0

    current_features = np.array([[score, price_ratio, brand_match]])
    prediction = rf_model.predict(current_features)[0]
    probability = rf_model.predict_proba(current_features)[0][1]

    print("---")
    print(f"Similarity Score: %{score*100:.2f}")
    print(f"Price Similarity Score: %{price_ratio*100:.2f}")
    print(f"RF Trust Probability: %{probability*100:.2f}")
    print("---")
    while (1):
        if prediction == 1:
            print("Products are Same ")
            break
            if priceTag1_num < priceTag2_num:
                print(f"{product_name1} is cheaper")
                break
            else:
                print(f"{product_name2} is cheaper")
                break
        else:
            print("Products are not the same")
            break
except Exception as error:
    print(f"Warning occured = {error}")
    name = "Unknown Product"

finally:
    driver.quit()
    print(f"[DEBUG] Raw Prices: {price1_tag} | {price2_tag}")
    print("\nOperation is successful")
